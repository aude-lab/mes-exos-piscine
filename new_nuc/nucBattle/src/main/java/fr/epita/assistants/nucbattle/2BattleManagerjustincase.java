package fr.epita.assistants.nucbattle;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.module.SimpleModule;

import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.Optional;

public class BattleManager {
    
    private final String inputPath;
    private final ObjectMapper mapper;

    public BattleManager(String path) {
        this.inputPath = path;
        this.mapper = new ObjectMapper();
    }

    public void computeBattle(String reportPath) throws IOException {
        Report report;
        
        try {
            JsonNode rootNode = mapper.readTree(new File(inputPath));
            
            JsonNode nucsNode = rootNode.get("NUCs");
            JsonNode battleNode = rootNode.get("battle");
            
            if (nucsNode == null || battleNode == null) {
                report = new Report();
                report.setType(ReportType.ERROR);
                mapper.writeValue(new File(reportPath), report);
                return;
            }
            
            JsonNode battleIdNode = battleNode.get("battle_id");
            if (battleIdNode == null || !battleIdNode.isInt()) {
                report = new Report();
                report.setType(ReportType.ERROR);
                mapper.writeValue(new File(reportPath), report);
                return;
            }
            
            Map<String, Nuc> nucsMap = new HashMap<>();
            
            nucsNode.fields().forEachRemaining(entry -> {
                try {
                    String login = entry.getKey();
                    Nuc nuc = mapper.treeToValue(entry.getValue(), Nuc.class);
                    nucsMap.put(login, nuc);
                } catch (IOException e) {
                    throw new RuntimeException(e);
                }
            });
            
            SimpleModule module = new SimpleModule();
            module.addDeserializer(Turn.class, new TurnDeserializer(nucsMap));
            mapper.registerModule(module);
            
            Battle battle = mapper.treeToValue(battleNode, Battle.class);
            
            report = simulateBattle(battle, nucsMap);
            
        } catch (Exception e) {
            report = new Report();
            report.setType(ReportType.ERROR);
        }
        
        mapper.writeValue(new File(reportPath), report);
    }

    private Report simulateBattle(Battle battle, Map<String, Nuc> nucsMap) {
        Optional<String> initialWinner = determWinner(nucsMap);
        if (initialWinner.isPresent()) {
            Report report = new Report();
            report.setType(ReportType.WINNER);
            report.setPlayer(initialWinner.get());
            report.setOutcome(computeFinalStatus(nucsMap));
            return report;
        }

        for (Turn turn : battle.getTurns()) {

            if (detectCheat(turn, nucsMap))
            {
                Report report = new Report();
                report.setType(ReportType.CHEATER);
                report.setPlayer(turn.getPlayerLogin());
                return report;
            }

            processTurnEffect(turn, nucsMap);

            Optional<String> theWinner = determWinner(nucsMap);
            if (theWinner.isPresent()) {
                Report report = new Report();
                report.setType(ReportType.WINNER);
                report.setPlayer(theWinner.get());
                report.setOutcome(computeFinalStatus(nucsMap));
                return report;
            }
        }


        Report report = new Report();
        report.setType(ReportType.UNFINISHED);
        report.setOutcome(computeFinalStatus(nucsMap));
        return report;
    }

    private boolean detectCheat(Turn turn, Map<String, Nuc> nucsMap) {
        Nuc playerNuc = nucsMap.get(turn.getPlayerLogin());
        if (playerNuc == null) {
            return true;
        }
        return turn.getPacket().getUsedPrograms().stream()
                .anyMatch(program -> !playerNuc.getInstalledPrograms().contains(program));
    }


    private void processTurnEffect(Turn turn, Map<String, Nuc> nucsMap) {
        Nuc target = nucsMap.get(turn.getTargetLogin());
        if (target == null) {
            return;
        }

        float currentHp = target.getHp();
        float damage = turn.getPacket().getDamage();
        float newHp = currentHp - damage;

        newHp = Math.max(0.0f, Math.min(100.0f, newHp));

        Nuc updatedNuc = new Nuc(target.getName(), newHp, target.getInstalledPrograms());

        nucsMap.put(turn.getTargetLogin(), updatedNuc);
    }


    private Optional<String> determWinner(Map<String, Nuc> nucsMap) {
        long playersAlive = nucsMap.values().stream()
                .filter(nuc -> nuc.getHp() > 0.0f)
                .count();

        if (playersAlive == 1) {
            String winner = nucsMap.entrySet().stream()
                    .filter(e -> e.getValue().getHp() > 0.0f)
                    .findFirst()
                    .get()
                    .getKey();

            return Optional.of(winner);
        }

        return Optional.empty();
    }


    private Map<String, Float> computeFinalStatus(Map<String, Nuc> nucsMap) {
        Map<String, Float> outcome = new HashMap<>();
        nucsMap.forEach((login, nuc) -> outcome.put(login, nuc.getHp()));
        return outcome;
    }
}

