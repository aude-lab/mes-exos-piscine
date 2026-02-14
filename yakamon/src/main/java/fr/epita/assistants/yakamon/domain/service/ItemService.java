package fr.epita.assistants.yakamon.domain.service;

import fr.epita.assistants.yakamon.converter.ModelConverter;
import fr.epita.assistants.yakamon.data.model.ItemModel;
import fr.epita.assistants.yakamon.data.repository.ItemRepository;
import fr.epita.assistants.yakamon.domain.entity.ItemEntity;
import fr.epita.assistants.yakamon.utils.tile.ItemType;
import jakarta.enterprise.context.ApplicationScoped;
import jakarta.inject.Inject;
import jakarta.transaction.Transactional;

import java.util.List;

@ApplicationScoped
public class ItemService {

    @Inject
    ItemRepository itemRepository;

    @Inject
    ModelConverter modelConverter;


    public List<ItemEntity> getAllItems() {
        List<ItemModel> models = itemRepository.findAllItems();
        return modelConverter.toItemEntityList(models);
    }


    public ItemEntity getItemByType(ItemType type) {
        ItemModel model = itemRepository.findByType(type);
        return modelConverter.toItemEntity(model);
    }


    @Transactional
    public void addItem(ItemType type, Integer quantity) {
        ItemModel model = itemRepository.findByType(type);

        if (model == null) {

            model = new ItemModel();
            model.setType(type);
            model.setQuantity(quantity);
        } else {
            model.setQuantity(model.getQuantity() + quantity);
        }

        itemRepository.saveItem(model);
    }

    @Transactional
    public void removeItem(ItemType type, Integer quantity) {
        ItemModel model = itemRepository.findByType(type);

        if (model != null) {
            Integer newQuantity = model.getQuantity() - quantity;

            if (newQuantity < 0) {
                newQuantity = 0;
            }

            model.setQuantity(newQuantity);
            itemRepository.saveItem(model);
        }
    }

    public boolean hasEnoughItems(ItemType type, Integer requiredQuantity) {
        ItemModel model = itemRepository.findByType(type);

        if (model == null) {
            return false;
        }

        return model.getQuantity() >= requiredQuantity;
    }

    @Transactional
    public void deleteAll() {
        itemRepository.deleteAll();
    }
}
