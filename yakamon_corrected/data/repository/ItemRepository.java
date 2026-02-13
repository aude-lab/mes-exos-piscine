package fr.epita.assistants.yakamon.data.repository;

import fr.epita.assistants.yakamon.data.model.ItemModel;
import fr.epita.assistants.yakamon.utils.tile.ItemType;
import io.quarkus.hibernate.orm.panache.PanacheRepositoryBase;
import jakarta.enterprise.context.ApplicationScoped;

import java.util.List;

@ApplicationScoped
public class ItemRepository implements PanacheRepositoryBase<ItemModel, Integer> {

    /**
     * ✅ Trouve un item par son type
     */
    public ItemModel findByType(ItemType type) {
        return find("type", type).firstResult();
    }

    /**
     * ✅ Retourne tous les items
     */
    public List<ItemModel> findAllItems() {
        return listAll();
    }

    /**
     * ✅ Sauvegarde un item
     */
    public void saveItem(ItemModel item) {
        persist(item);
    }

    /**
     * ✅ Supprime tous les items
     */
    public void deleteAll() {
        delete("1=1");
    }
}
