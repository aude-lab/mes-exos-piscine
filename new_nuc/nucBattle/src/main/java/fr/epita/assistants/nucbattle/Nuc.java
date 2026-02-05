package fr.epita.assistants.nucbattle;

import com.fasterxml.jackson.annotation.JsonProperty;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;

import java.util.List;

@Getter
@AllArgsConstructor
@NoArgsConstructor
public class Nuc {
    private String name;

    private float hp;

    @JsonProperty("installed_programs")
    private List<String> installedPrograms;
}
