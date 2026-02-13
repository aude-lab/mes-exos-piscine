package fr.epita.assistants.yakamon.data.repository;

import fr.epita.assistants.yakamon.data.model.GameModel;
import fr.epita.assistants.yakamon.domain.entity.GameEntity;
import io.quarkus.hibernate.orm.panache.PanacheRepositoryBase;
import jakarta.enterprise.context.ApplicationScoped;

@ApplicationScoped
public class GameRepository implements PanacheRepositoryBase<GameModel, Integer> {

    public GameEntity findGame() {
        GameModel model = listAll().stream().findFirst().orElse(null);
        return model != null ? toEntity(model) : null;
    }

    public GameEntity saveGame(GameEntity entity) {
        GameModel model = toModel(entity);
        persist(model);
        return toEntity(model);
    }

    public void deleteAll() {
        delete("1=1");
    }
    
    
    private GameEntity toEntity(GameModel model) {
        GameEntity entity = new GameEntity();
        entity.setId(model.getId());
        entity.setMap(model.getMap());
        return entity;
    }
    
    private GameModel toModel(GameEntity entity) {
        GameModel model = new GameModel();
        model.setId(entity.getId());
        model.setMap(entity.getMap());
        return model;
    }
}
