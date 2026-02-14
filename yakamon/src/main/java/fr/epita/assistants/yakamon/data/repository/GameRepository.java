package fr.epita.assistants.yakamon.data.repository;

import fr.epita.assistants.yakamon.data.model.GameModel;
import io.quarkus.hibernate.orm.panache.PanacheRepositoryBase;
import jakarta.enterprise.context.ApplicationScoped;

@ApplicationScoped
public class GameRepository implements PanacheRepositoryBase<GameModel, Integer> {


    public GameModel findGame() {
        return listAll().stream().findFirst().orElse(null);
    }


    public GameModel saveGame(GameModel model) {
        persist(model);
        return model;
    }

    public void deletedAll() {
        delete("1=1");
    }
}