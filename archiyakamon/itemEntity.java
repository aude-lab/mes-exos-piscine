package fr.epita.assistants.yakamon.domain.entity;

import fr.epita.assistants.yakamon.utils.tile.ItemType;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@NoArgsConstructor
@AllArgsConstructor
public class ItemEntity {

    private Integer id;

    private ItemType type;

    private Integer quantity;
}
