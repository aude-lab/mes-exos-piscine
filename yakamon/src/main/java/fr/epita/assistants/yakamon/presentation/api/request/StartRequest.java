package fr.epita.assistants.yakamon.presentation.api.request;
import com.fasterxml.jackson.annotation.JsonProperty;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;
@AllArgsConstructor
@NoArgsConstructor
@Data
public class StartRequest {
    @JsonProperty("mapPath")
    private String mapPath;
    @JsonProperty("playerName")
    private String playerName;
}
