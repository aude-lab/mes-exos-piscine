package fr.epita.assistants.nucbattle;

import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.module.SimpleModule;
import lombok.*;

import java.io.File;
import java.io.IOException;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.stream.Collectors;

@Getter
public class BattleManager {
    private final String inputPath;
    private final ObjectMapper mapper = new ObjectMapper();

    public BattleManager(String inputPath) {
        this.inputPath = inputPath;
    }

    public void computeBattle(String reportPath) throws IOException {
        try {
            Map<String, Object> input = mapper.readValue(new File(inputPath), new TypeReference<>() {});
            Map<String, Nuc> nucs = loadNucs(input);

            SimpleModule module = new SimpleModule();
            module.addDeserializer(Turn.class, new TurnDeserializer(nucs));
            mapper.registerModule(module);

            Battle battle = mapper.convertValue(input.get("battle"), Battle.class);
            Report report = processBattle(battle.getTurns(), nucs);

            mapper.writeValue(new File(reportPath), report);
        } catch (Exception e) {
            mapper.writeValue(new File(reportPath), new Report(ReportType.ERROR));
        }
    }

    private Map<String, Nuc> loadNucs(Map<String, Object> input) {
        Map<String, Map<String, Object>> nucsMap = (Map<String, Map<String, Object>>) input.get("NUCs");

        return nucsMap.entrySet().stream().collect(Collectors.toMap(Map.Entry::getKey, e -> mapper.convertValue(e.getValue(), Nuc.class)));
    }

    private Report processBattle(List<Turn> turns, Map<String, Nuc> nucs) {
        try {
            for (Turn turn : turns) {
                if (isCheater(turn, nucs)) {
                    return new Report(ReportType.CHEATER, turn.getPlayerLogin());
                }

                applyDamage(turn, nucs);

                Optional<String> winner = checkWinner(nucs);
                if (winner.isPresent()) {
                    return new Report(ReportType.WINNER, winner.get(), getOutcome(nucs));
                }
            }

            // Vérification finale après tous les tours
            Optional<String> finalWinner = checkWinner(nucs);
            return finalWinner.isPresent() ? new Report(ReportType.WINNER, finalWinner.get(), getOutcome(nucs)) : new Report(ReportType.UNFINISHED, getOutcome(nucs));
        } catch (Exception e) {
            return new Report(ReportType.ERROR);
        }
    }

    private boolean isCheater(Turn turn, Map<String, Nuc> nucs) {
        Nuc palyerNuc = nucs.get(turn.getPlayerLogin());
        if (palyerNuc == null) {
            return true;
        }
        return turn.getPacket().getUsedPrograms().stream().anyMatch(program ->!palyerNuc.getInstalledPrograms().contains(program));
    }

    private Optional<String> checkWinner(Map<String, Nuc> nucs) {
        List<Map.Entry<String, Nuc>> alive = nucs.entrySet().stream()
                .filter(e -> e.getValue().getHp() > 0)
                .toList();

        if (alive.size() == 1) {
            // Vérifie que tous les autres ont bien 0 HP
            boolean allOthersDead = nucs.values().stream().filter(n -> n.getHp() <= 0).count() == nucs.size() - 1;

            return allOthersDead ? Optional.of(alive.get(0).getKey()) : Optional.empty();
        }
        return Optional.empty();
    }

    private void applyDamage(Turn turn, Map<String, Nuc> nucs) {
        Nuc target = nucs.get(turn.getTargetLogin());
        if (target == null){
            return;
        }

        float currentHp = Math.max(0, Math.min(100, target.getHp()));
        float newHp = currentHp - turn.getPacket().getDamage();
        newHp = Math.max(0, Math.min(100, newHp));

        Nuc updatedNuc = new Nuc(target.getName(), newHp, target.getInstalledPrograms());
        nucs.put(turn.getTargetLogin(), updatedNuc);
    }

    private Map<String, Float> getOutcome(Map<String, Nuc> nucs) {
        return nucs.entrySet().stream().collect(Collectors.toMap(Map.Entry::getKey, e -> e.getValue().getHp()));
    }
}