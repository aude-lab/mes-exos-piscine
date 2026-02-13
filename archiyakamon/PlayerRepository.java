package fr.epita.assistants.yakamon.data.repository;

import fr.epita.assistants.yakamon.converter.ModelConverter;
import fr.epita.assistants.yakamon.data.model.PlayerModel;
import fr.epita.assistants.yakamon.domain.entity.PlayerEntity;
import io.quarkus.hibernate.orm.panache.PanacheRepositoryBase;
import jakarta.enterprise.context.ApplicationScoped;
import jakarta.inject.Inject;

import java.util.UUID;

@ApplicationScoped
public class PlayerRepository implements PanacheRepositoryBase<PlayerModel, UUID> {
@Inject
    ModelConverter modelConverter;    
    public PlayerEntity findPlayer() {
        PlayerModel model = listAll().stream().findFirst().orElse(null);
        return model != null ? toEntity(model) : null;
    }
    
    public PlayerEntity savePlayer(PlayerEntity entity) {
        PlayerModel model = toModel(entity);
        persist(model);
        return toEntity(model);
    }
    
    public void deleteAll() {
        delete("1=1");
    }
    
}
