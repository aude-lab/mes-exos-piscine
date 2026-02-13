package fr.epita.assistants.yakamon.data.model;

import jakarta.persistence.*;
import lombok.Getter;
import lombok.Setter;

import java.time.LocalDateTime;
import java.util.UUID;

@Entity
@Table(name = "player")
@Getter
@Setter
public class PlayerModel {
    @Id
    @GeneratedValue(strategy = GenerationType.UUID)
    private UUID uuid;

    @Column(nullable = false)
    private String name;

    @Column(name = "pos_x", nullable = false)
    private Integer posX;

    @Column(name = "pos_y", nullable = false)
    private Integer posY;

    @Column(name = "last_move")
    private LocalDateTime lastMove;

    @Column(name = "last_catch")
    private LocalDateTime lastCatch;

    @Column(name = "last_collect")
    private LocalDateTime lastCollect;

    @Column(name = "last_feed")
    private LocalDateTime lastFeed;

}
