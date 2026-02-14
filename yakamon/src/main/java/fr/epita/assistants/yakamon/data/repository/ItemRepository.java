package fr.epita.assistants.yakamon.data.repository;

import fr.epita.assistants.yakamon.data.model.ItemModel;
import fr.epita.assistants.yakamon.utils.tile.ItemType;
import io.quarkus.hibernate.orm.panache.PanacheRepositoryBase;
import jakarta.enterprise.context.ApplicationScoped;

import java.util.List;

@ApplicationScoped
public class ItemRepository implements PanacheRepositoryBase<ItemModel, Integer> {


    public ItemModel findByType(ItemType type) {
        return find("type", type).firstResult();
    }


    public List<ItemModel> findAllItems() {
        return listAll();
    }

    public void saveItem(ItemModel item) {
        persist(item);
    }


    public void deletedAll() {
        delete("1=1");
    }
}
