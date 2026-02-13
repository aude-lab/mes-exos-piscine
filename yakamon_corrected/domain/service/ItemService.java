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
    ModelConverter modelConverter;  // ✅ Injecte le converter

    /**
     * ✅ Récupère tous les items
     * - Repository retourne List<ItemModel>
     * - Converter transforme en List<ItemEntity>
     */
    public List<ItemEntity> getAllItems() {
        List<ItemModel> models = itemRepository.findAllItems();
        return modelConverter.toItemEntityList(models);
    }

    /**
     * ✅ Récupère un item par son type
     * - Repository retourne ItemModel
     * - Converter transforme en ItemEntity
     */
    public ItemEntity getItemByType(ItemType type) {
        ItemModel model = itemRepository.findByType(type);
        return modelConverter.toItemEntity(model);
    }

    /**
     * ✅ Ajoute des items à l'inventaire
     * - Travaille avec ItemEntity dans le service
     * - Convertit en ItemModel pour sauvegarder
     */
    @Transactional
    public void addItem(ItemType type, Integer quantity) {
        ItemModel model = itemRepository.findByType(type);

        if (model == null) {
            // Créer un nouvel item
            model = new ItemModel();
            model.setType(type);
            model.setQuantity(quantity);
        } else {
            // Augmenter la quantité
            model.setQuantity(model.getQuantity() + quantity);
        }

        itemRepository.saveItem(model);
    }

    /**
     * ✅ Retire des items de l'inventaire
     */
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

    /**
     * ✅ Vérifie si le joueur a assez d'items
     */
    public boolean hasEnoughItems(ItemType type, Integer requiredQuantity) {
        ItemModel model = itemRepository.findByType(type);

        if (model == null) {
            return false;
        }

        return model.getQuantity() >= requiredQuantity;
    }

    /**
     * Supprime tous les items
     */
    @Transactional
    public void deleteAll() {
        itemRepository.deleteAll();
    }
}
