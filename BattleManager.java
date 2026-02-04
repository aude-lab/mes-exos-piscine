package fr.epita.assistants.nucbattle;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.module.SimpleModule;

import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

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
            
            Battle battle = mapper.treeToValue(rootNode.get("battle"), Battle.class);
            
            report = simulateBattle(battle, nucsMap);
            
        } catch (Exception e) {
            report = new Report();
            report.setType(ReportType.ERROR);
        }
        
        mapper.writeValue(new File(reportPath), report);
    }

    private Report simulateBattle(Battle battle, Map<String, Nuc> nucsMap) {
        Map<String, Float> hpMap = new HashMap<>();
        nucsMap.forEach((login, nuc) -> hpMap.put(login, nuc.getHp()));
	Report initialCheck = checkForWinner(hpMap);
        if (initialCheck != null) {
            return initialCheck;
        }
        
        for (Turn turn : battle.getTurns()) {
            Packet packet = turn.getPacket();
            if (!turn.getPlayerNuc().getInstalledPrograms().containsAll(packet.getUsedPrograms())) {
                Report report = new Report();
                report.setType(ReportType.CHEATER);
                report.setPlayer(turn.getPlayerLogin());
                return report;
            }
            
            String targetLogin = turn.getTargetLogin();
            float currentHp = hpMap.get(targetLogin);
            float newHp = currentHp - packet.getDamage();
            newHp = Math.max(0.0f, Math.min(100.0f, newHp));
            hpMap.put(targetLogin, newHp);

	    Report winnerCheck = checkForWinner(hpMap);
            if (winnerCheck != null) {
                return winnerCheck;
            }
	}
	Report report = new Report();
        report.setType(ReportType.UNFINISHED);
        report.setOutcome(hpMap);
        return report;
    }

    private Report checkForWinner(Map<String, Float> hpMap) {
        long playersAlive = hpMap.values().stream()
            .filter(hp -> hp > 0.0f)
            .count();

        if (playersAlive == 1) {
            String winner = hpMap.entrySet().stream()
                .filter(e -> e.getValue() > 0.0f)
                .findFirst()
                .get()
                .getKey();

            Report report = new Report();
            report.setType(ReportType.WINNER);
            report.setPlayer(winner);
            report.setOutcome(hpMap);
            return report;
        }

        return null;

    }
}
