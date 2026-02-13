package fr.epita.assistants.yakamon.domain.service;
import fr.epita.assistants.yakamon.data.model.ItemModel;
import fr.epita.assistants.yakamon.data.repository.ItemRepository;
import fr.epita.assistants.yakamon.utils.tile.ItemType;
import jakarta.enterprise.context.ApplicationScoped;
import jakarta.inject.Inject;
import jakarta.transaction.Transactional;

import java.util.List;
@ApplicationScoped

public class ItemService {

    @Inject
    ItemRepository itemRepository;

    public List<ItemModel> getAllItems() {
        return itemRepository.findAllItems();
    }
    public ItemModel getItemByType(ItemType type) {
        return itemRepository.findByType(type);
    }

    @Transactional
    public void addItem(ItemType type, Integer quantity) {
        ItemModel item = itemRepository.findByType(type);

        if (item == null)
        {
            item = new ItemModel();
            item.setType(type);
            item.setQuantity(quantity);
        } else {
            item.setQuantity(item.getQuantity() + quantity);
        }

        itemRepository.saveItem(item);
    }

    @Transactional
    public void removeItem(ItemType type, Integer quantity) {
        ItemModel item = itemRepository.findByType(type);

        if (item != null) {
            Integer newQuantity = item.getQuantity() - quantity;

            if (newQuantity < 0) {
                newQuantity = 0;
            }
            item.setQuantity(newQuantity);
            itemRepository.saveItem(item);
        }
    }

    public boolean hasEnoughItems(ItemType type, Integer requiredQuantity) {
        ItemModel item = itemRepository.findByType(type);

        if (item == null) {
            return false;
        }

        return item.getQuantity() >= requiredQuantity;
    }

    @Transactional
    public void deleteAll()
    {
        itemRepository.deleteAll();
    }




}
