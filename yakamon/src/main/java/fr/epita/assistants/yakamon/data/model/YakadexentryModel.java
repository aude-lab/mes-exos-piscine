package fr.epita.assistants.yakamon.data.model;

import fr.epita.assistants.yakamon.utils.ElementType;
import jakarta.persistence.*;
import lombok.Getter;
import lombok.Setter;
import lombok.AllArgsConstructor;
import lombok.NoArgsConstructor;
@Entity
@Table(name = "yakadex_entry")
@Getter
@Setter
@NoArgsConstructor
@AllArgsConstructor

public class YakadexentryModel {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Integer id;

    @Column(name = "name", length = 20, nullable = false)
    private String name;

    @Enumerated(EnumType.STRING)
    @Column(name = "first_type", nullable = false)
    private ElementType firstType;

    @Enumerated(EnumType.STRING)
    @Column(name = "second_type")
    private ElementType secondType;

    @Column(name = "description", columnDefinition = "TEXT")
    private String description;

    @Column(name = "evolution_id")
    private Integer evolutionId;

    @Column(name = "evolve_threshold")
    private Integer evolveThreshold;

    @Column(name = "caught", nullable = false)
    private Boolean caught;


}