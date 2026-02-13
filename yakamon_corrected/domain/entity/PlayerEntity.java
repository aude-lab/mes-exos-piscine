package fr.epita.assistants.yakamon.domain.entity;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.time.LocalDateTime;
import java.util.UUID;

@Data
@NoArgsConstructor
@AllArgsConstructor
public class PlayerEntity {
    private UUID uuid;
    private String name;
    private Integer posX;
    private Integer posY;
    private LocalDateTime lastMove;
    private LocalDateTime lastCatch;
    private LocalDateTime lastCollect;
    private LocalDateTime lastFeed;  // ✅ Corrigé : était "LastFeed" (majuscule) au lieu de "lastFeed"
}
