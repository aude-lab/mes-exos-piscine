package fr.epita.assistants.nucbattle;

import com.fasterxml.jackson.annotation.JsonProperty;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;

import java.util.List;

@Getter
@AllArgsConstructor
@NoArgsConstructor
public class Battle {
    @JsonProperty("battle_id")
    private int battleId;

    private List<Turn> turns;
}
