package fr.epita.assistants.nucbattle;

import com.fasterxml.jackson.annotation.JsonProperty;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;

import java.util.List;

@Getter
@AllArgsConstructor
@NoArgsConstructor
public class Packet {
    @JsonProperty("used_programs")
    private List<String> usedPrograms;

    private String command;

    private float damage;
}
