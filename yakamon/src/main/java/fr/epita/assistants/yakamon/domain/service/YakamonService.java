package fr.epita.assistants.yakamon.domain.service;

import fr.epita.assistants.yakamon.converter.ModelConverter;
import fr.epita.assistants.yakamon.data.model.YakamonModel;
import fr.epita.assistants.yakamon.data.repository.YakamonRepository;
import fr.epita.assistants.yakamon.domain.entity.YakamonEntity;
import jakarta.enterprise.context.ApplicationScoped;
import jakarta.inject.Inject;
import jakarta.transaction.Transactional;
import java.util.List;
import java.util.UUID;
import java.util.stream.Collectors;

@ApplicationScoped
public class YakamonService {

    @Inject
    YakamonRepository yakamonRepository;

    @Inject
    ModelConverter modelConverter;

    public List<YakamonEntity> getAllYakamons() {
        List<YakamonModel> models = yakamonRepository.findAllYakamons();
        return modelConverter.toYakamonEntityList(models);
    }

    public YakamonEntity getYakamonByUuid(UUID uuid) {
        YakamonModel model = yakamonRepository.findByUuid(uuid);
        return modelConverter.toYakamonEntity(model);
    }

    @Transactional
    public YakamonEntity createYakamon(String nickname, Integer energyPoints, Integer yakadexEntryId) {
        YakamonEntity entity = new YakamonEntity();
        entity.setNickname(nickname);
        entity.setEnergyPoints(energyPoints);
        entity.setYakadexEntryId(yakadexEntryId);

        YakamonModel model = modelConverter.toYakamonModel(entity);
        YakamonModel saved = yakamonRepository.saveYakamon(model);

        return modelConverter.toYakamonEntity(saved);
    }

    @Transactional
    public void deleteYakamon(UUID uuid) {
        yakamonRepository.deleteYakamon(uuid);
    }

    public long getTeamSize() {
        return yakamonRepository.countYakamons();
    }

    public boolean yakamonExists(UUID uuid) {
        return yakamonRepository.findByUuid(uuid) != null;
    }
}
