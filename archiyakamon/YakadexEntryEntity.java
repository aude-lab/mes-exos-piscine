package fr.epita.assistants.yakamon.domain.entity;

import fr.epita.assistants.yakamon.utils.ElementType;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * Entity représentant une entrée du Yakadex dans la couche BUSINESS.
 * 
 * Représente une espèce de yakamon.
 */
@Data
@NoArgsConstructor
@AllArgsConstructor
public class YakadexEntryEntity {

    private Integer id;

    private String name;

    private ElementType firstType;

    private ElementType secondType;

    private String description;

    private Integer evolutionId;

    private Integer evolveThreshold;

    private Boolean caught;
}
