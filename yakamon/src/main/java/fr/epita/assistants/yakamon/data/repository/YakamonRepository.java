package fr.epita.assistants.yakamon.data.repository;

import fr.epita.assistants.yakamon.data.model.YakamonModel;
import io.quarkus.hibernate.orm.panache.PanacheRepositoryBase;
import jakarta.enterprise.context.ApplicationScoped;
import java.util.List;
import java.util.UUID;

@ApplicationScoped
public class YakamonRepository implements PanacheRepositoryBase<YakamonModel, UUID> {

    public List<YakamonModel> findAllYakamons() {
        return listAll();
    }

    public YakamonModel findByUuid(UUID uuid) {
        return findById(uuid);
    }

    public YakamonModel saveYakamon(YakamonModel model) {
        persist(model);
        return model;
    }

    public void deleteYakamon(UUID uuid) {
        deleteById(uuid);
    }

    public long countYakamons() {
        return count();
    }
}