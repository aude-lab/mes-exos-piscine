package fr.epita.assistants.yakamon.data.model;

import jakarta.persistence.*;
import lombok.Getter;
import lombok.Setter;
import java.util.UUID;

@Entity
@Table(name = "yakamon")
@Getter
@Setter
public class YakamonModel {

    @Id
    @GeneratedValue(strategy = GenerationType.UUID)
    @Column(name = "uuid")
    private UUID uuid;

    @Column(name = "nickname", length = 20, nullable = false)
    private String nickname;

    @Column(name = "energy_points", nullable = false)
    private Integer energyPoints;

    @Column(name = "yakadex_entry_id", nullable = false)
    private Integer yakadexEntryId;
}
