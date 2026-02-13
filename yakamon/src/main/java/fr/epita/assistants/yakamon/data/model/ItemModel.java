package fr.epita.assistants.yakamon.data.model;

import fr.epita.assistants.yakamon.utils.tile.ItemType;
import jakarta.persistence.*;
import lombok.Getter;
import lombok.Setter;

@Entity
@Table(name = "item")
@Getter
@Setter
public class ItemModel {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Integer id;
    @Enumerated(EnumType.STRING)
    @Column(name = "type", nullable = false, unique = true)
    private ItemType type;
    @Column(name = "quantity", nullable = false)
    private Integer quantity;
}
