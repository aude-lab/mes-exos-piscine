package fr.epita.assistants.yakamon.domain.service;

import fr.epita.assistants.yakamon.converter.ModelConverter;
import fr.epita.assistants.yakamon.data.model.YakadexentryModel;
import fr.epita.assistants.yakamon.data.repository.YakadexEntryRepository;
import fr.epita.assistants.yakamon.domain.entity.YakadexEntryEntity;
import jakarta.enterprise.context.ApplicationScoped;
import jakarta.inject.Inject;
import jakarta.transaction.Transactional;

import java.util.List;

@ApplicationScoped
public class YakadexEntryService {

    @Inject
    YakadexEntryRepository yakadexRepository;

    @Inject
    ModelConverter modelConverter;  // ✅ Injecte le converter

    /**
     * ✅ Récupère toutes les entrées du Yakadex
     * - Repository retourne List<YakadexentryModel>
     * - Converter transforme en List<YakadexEntryEntity>
     */
    public List<YakadexEntryEntity> getAllEntries() {
        List<YakadexentryModel> models = yakadexRepository.findAllEntries();
        return modelConverter.toYakadexEntryEntityList(models);
    }

    /**
     * ✅ Récupère les yakamons non capturés
     */
    public List<YakadexEntryEntity> getUncaughtEntries() {
        List<YakadexentryModel> models = yakadexRepository.findUncaughtEntries();
        return modelConverter.toYakadexEntryEntityList(models);
    }

    /**
     * ✅ Récupère une entrée par ID
     * - Repository retourne YakadexentryModel
     * - Converter transforme en YakadexEntryEntity
     */
    public YakadexEntryEntity getEntryById(Integer id) {
        YakadexentryModel model = yakadexRepository.findById(id);
        return modelConverter.toYakadexEntryEntity(model);
    }

    /**
     * ✅ Marque un yakamon comme capturé
     */
    @Transactional
    public void markAsCaught(Integer yakadexId) {
        YakadexentryModel model = yakadexRepository.findById(yakadexId);

        if (model != null) {
            model.setCaught(true);
            yakadexRepository.saveEntry(model);
        }
    }

    /**
     * ✅ Réinitialise tous les yakamons comme non capturés
     */
    @Transactional
    public void resetAllCaught() {
        yakadexRepository.resetAllCaught();
    }
}
