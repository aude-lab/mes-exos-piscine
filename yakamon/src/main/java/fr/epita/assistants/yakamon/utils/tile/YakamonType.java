package fr.epita.assistants.yakamon.utils.tile;

import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonFormat;
import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.databind.JsonNode;
import lombok.AllArgsConstructor;

@JsonFormat(shape = JsonFormat.Shape.OBJECT)
@AllArgsConstructor
public enum YakamonType implements Collectible {
    LUCARYON(new YakamonInfo('u').withYakadexId(8)),
    RAYQUAÏSSA(new YakamonInfo('r').withYakadexId(7)),
    BASTIENDON(new YakamonInfo('b').withYakadexId(6)),
    GROUVAN(new YakamonInfo('g').withYakadexId(5)),
    LUCARYA(new YakamonInfo('l').withYakadexId(4)),
    LOUICUNE(new YakamonInfo('o').withYakadexId(3)),
    NICOTALI(new YakamonInfo('n').withYakadexId(2)),
    YAKORYU(new YakamonInfo('y').withYakadexId(1));

    private final CollectibleInfo collectibleInfo;

    @Override
    public CollectibleType getCollectibleType() {
        return CollectibleType.YAKAMON;
    }

    @Override
    public String getValue() {
        return name();
    }

    @JsonIgnore
    @Override
    public CollectibleInfo getCollectibleInfo() {
        return collectibleInfo;
    }

    @JsonCreator
    public static YakamonType jsonCreator(JsonNode node) {
        String value = node.get("value").asText();
        return YakamonType.valueOf(value);
    }
}
