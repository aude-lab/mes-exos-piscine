package fr.epita.assistants.nucbattle;

import com.fasterxml.jackson.core.JacksonException;
import com.fasterxml.jackson.core.JsonParser;
import com.fasterxml.jackson.databind.DeserializationContext;
import com.fasterxml.jackson.databind.JsonDeserializer;
import com.fasterxml.jackson.databind.JsonNode;
import java.io.IOException;
import java.util.Map;
import lombok.*;

@Getter
@AllArgsConstructor
public class TurnDeserializer extends JsonDeserializer<Turn> {
    private final Map<String, Nuc> nucs;

    @Override
    public Turn deserialize(JsonParser p, DeserializationContext ctxt) throws IOException {
        JsonNode node = p.getCodec().readTree(p);
        String playerLogin = node.get("player_login").asText();
        String targetLogin = node.get("target_login").asText();

        // Vérifie que les logins existent dans la map
        if (!nucs.containsKey(playerLogin) || !nucs.containsKey(targetLogin)) {
            throw new IOException("Invalid player or target login");
        }

        Packet packet = p.getCodec().treeToValue(node.get("packet"), Packet.class);
        return new Turn(nucs.get(playerLogin), playerLogin, nucs.get(targetLogin), targetLogin, packet);
    }
}
