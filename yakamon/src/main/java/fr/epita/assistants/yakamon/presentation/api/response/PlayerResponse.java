package fr.epita.assistants.yakamon.presentation.api.response;
import com.fasterxml.jackson.annotation.JsonInclude;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;
import java.time.LocalDateTime;
import java.util.UUID;
@Data
@AllArgsConstructor
@NoArgsConstructor
@JsonInclude(JsonInclude.Include.NON_NULL)
public class PlayerResponse {
    private UUID id;
    private String name;
    private Integer posX;
    private Integer posY;
    private LocalDateTime lastMove;
    private LocalDateTime lastCollect;
    private LocalDateTime lastCatch;
    private LocalDateTime lastFeed;

}
