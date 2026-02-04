package fr.epita.assistants.nucbattle;

import com.fasterxml.jackson.core.JsonParser;
import com.fasterxml.jackson.databind.DeserializationContext;
import com.fasterxml.jackson.databind.JsonDeserializer;
import com.fasterxml.jackson.databind.JsonNode;

import java.io.IOException;
import java.util.Map;

public class TurnDeserializer extends JsonDeserializer<Turn> {
    
    private final Map<String, Nuc> nucs;

    public TurnDeserializer(Map<String, Nuc> nucs) {
        this.nucs = nucs;
    }

    @Override
    public Turn deserialize(JsonParser parser, DeserializationContext context) throws IOException {
        JsonNode node = parser.getCodec().readTree(parser);
        
        String playerLogin = node.get("player_login").asText();
        String targetLogin = node.get("target_login").asText();
        
        Nuc playerNuc = nucs.get(playerLogin);
        Nuc targetNuc = nucs.get(targetLogin);
        
        Packet packet = parser.getCodec().treeToValue(node.get("packet"), Packet.class);
        
        return new Turn(playerNuc, playerLogin, targetNuc, targetLogin, packet);
    }
}

