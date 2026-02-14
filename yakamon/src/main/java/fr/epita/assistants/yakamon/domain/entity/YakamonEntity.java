package fr.epita.assistants.yakamon.domain.entity;

import lombok.Getter;
import lombok.Setter;
import java.util.UUID;

@Getter
@Setter
public class YakamonEntity {
    private UUID uuid;
    private String nickname;
    private Integer energyPoints;
    private Integer yakadexEntryId;
}
