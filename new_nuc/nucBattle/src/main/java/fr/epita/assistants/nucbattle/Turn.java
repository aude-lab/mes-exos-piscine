package fr.epita.assistants.nucbattle;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;

@AllArgsConstructor
@NoArgsConstructor
@Getter
public class Turn {
    private Nuc playerNuc;
    private String playerLogin;

    private Nuc targetNuc;
    private String targetLogin;

    private Packet packet;
}
