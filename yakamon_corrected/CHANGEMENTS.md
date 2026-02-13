# 🔧 LISTE DES CHANGEMENTS À APPLIQUER

## 📋 Vue d'ensemble

Le problème principal était que **les Repositories faisaient des conversions Model ↔ Entity**, ce qui viole l'architecture en couches. Les conversions doivent être faites dans les **Services** via le **ModelConverter**.

---

## 🔴 FICHIERS À MODIFIER

### 1. **GameRepository.java** ✏️
**Emplacement :** `data/repository/GameRepository.java`

**Changements :**
- ❌ Supprimer les méthodes `toEntity()` et `toModel()`
- ❌ Supprimer l'injection de `GameConverter` si présente
- ✅ Faire retourner `GameModel` au lieu de `GameEntity`
- ✅ Prendre `GameModel` en paramètre au lieu de `GameEntity`

**Avant :**
```java
public GameEntity findGame() {
    GameModel model = listAll().stream().findFirst().orElse(null);
    return model != null ? toEntity(model) : null;  // ❌
}

private GameEntity toEntity(GameModel model) { ... }  // ❌
```

**Après :**
```java
public GameModel findGame() {
    return listAll().stream().findFirst().orElse(null);  // ✅
}
// Plus de méthode toEntity()
```

---

### 2. **PlayerRepository.java** ✏️
**Emplacement :** `data/repository/PlayerRepository.java`

**Changements :**
- ❌ Supprimer `@Inject ModelConverter`
- ❌ Supprimer l'utilisation de `modelConverter.toPlayerEntity()`
- ✅ Faire retourner `PlayerModel` au lieu de `PlayerEntity`

**Avant :**
```java
@Inject
ModelConverter modelConverter;  // ❌

public PlayerEntity findPlayer() {
    PlayerModel model = listAll().stream().findFirst().orElse(null);
    return model != null ? modelConverter.toPlayerEntity(model) : null;  // ❌
}
```

**Après :**
```java
// Plus d'injection de ModelConverter

public PlayerModel findPlayer() {
    return listAll().stream().findFirst().orElse(null);  // ✅
}
```

---

### 3. **ModelConverter.java** ✏️
**Emplacement :** `converter/ModelConverter.java`

**Changements :**
- ✅ Ajouter les conversions pour `ItemEntity` ↔ `ItemModel`
- ✅ Ajouter les conversions pour `YakadexEntryEntity` ↔ `YakadexentryModel`
- ✅ Ajouter les méthodes pour convertir des listes

**À ajouter :**
```java
// Item conversions
public ItemEntity toItemEntity(ItemModel model) { ... }
public ItemModel toItemModel(ItemEntity entity) { ... }
public List<ItemEntity> toItemEntityList(List<ItemModel> models) { ... }

// Yakadex conversions
public YakadexEntryEntity toYakadexEntryEntity(YakadexentryModel model) { ... }
public YakadexentryModel toYakadexEntryModel(YakadexEntryEntity entity) { ... }
public List<YakadexEntryEntity> toYakadexEntryEntityList(List<YakadexentryModel> models) { ... }
```

---

### 4. **GameService.java** ✏️
**Emplacement :** `domain/service/GameService.java`

**Changements :**
- ✅ Injecter `ModelConverter` si pas déjà fait
- ✅ Utiliser le converter dans `getGame()`
- ✅ Utiliser le converter dans `createGame()`

**Avant :**
```java
@Inject
GameConverter gameConverter;  // ❌ Mauvais converter

public GameEntity getGame() {
    return gameRepository.findGame();  // ❌ Repository retourne Entity
}
```

**Après :**
```java
@Inject
ModelConverter modelConverter;  // ✅ Bon converter

public GameEntity getGame() {
    GameModel model = gameRepository.findGame();
    return modelConverter.toGameEntity(model);  // ✅
}

@Transactional
public GameEntity createGame(String mapFilePath) {
    // ... parse map
    GameEntity entity = new GameEntity();
    entity.setMap(parsedMap);
    
    // Convertit Entity → Model
    GameModel model = modelConverter.toGameModel(entity);
    GameModel savedModel = gameRepository.saveGame(model);
    
    // Reconvertit Model → Entity
    return modelConverter.toGameEntity(savedModel);
}
```

---

### 5. **PlayerService.java** ✏️
**Emplacement :** `domain/service/PlayerService.java`

**Changements :**
- ✅ Injecter `ModelConverter`
- ✅ Utiliser le converter partout où on récupère ou sauvegarde un joueur

**Avant :**
```java
public PlayerEntity getPlayer() {
    return playerRepository.findPlayer();  // ❌ Repository retourne Entity
}

@Transactional
public PlayerEntity createPlayer(String name) {
    PlayerEntity player = new PlayerEntity();
    // ... set properties
    return playerRepository.savePlayer(player);  // ❌
}
```

**Après :**
```java
@Inject
ModelConverter modelConverter;  // ✅

public PlayerEntity getPlayer() {
    PlayerModel model = playerRepository.findPlayer();
    return modelConverter.toPlayerEntity(model);  // ✅
}

@Transactional
public PlayerEntity createPlayer(String name) {
    PlayerEntity entity = new PlayerEntity();
    // ... set properties
    
    PlayerModel model = modelConverter.toPlayerModel(entity);
    PlayerModel savedModel = playerRepository.savePlayer(model);
    return modelConverter.toPlayerEntity(savedModel);  // ✅
}
```

---

### 6. **ItemService.java** ✏️
**Emplacement :** `domain/service/ItemService.java`

**Changements :**
- ✅ Injecter `ModelConverter`
- ✅ Retourner `List<ItemEntity>` au lieu de `List<ItemModel>`
- ✅ Utiliser le converter pour toutes les opérations

**Avant :**
```java
public List<ItemModel> getAllItems() {
    return itemRepository.findAllItems();  // ❌ Retourne Models
}

public ItemModel getItemByType(ItemType type) {
    return itemRepository.findByType(type);  // ❌ Retourne Model
}
```

**Après :**
```java
@Inject
ModelConverter modelConverter;  // ✅

public List<ItemEntity> getAllItems() {
    List<ItemModel> models = itemRepository.findAllItems();
    return modelConverter.toItemEntityList(models);  // ✅
}

public ItemEntity getItemByType(ItemType type) {
    ItemModel model = itemRepository.findByType(type);
    return modelConverter.toItemEntity(model);  // ✅
}
```

---

### 7. **YakadexEntryService.java** ✏️
**Emplacement :** `domain/service/YakadexEntryService.java`

**Changements :**
- ✅ Injecter `ModelConverter`
- ✅ Retourner `List<YakadexEntryEntity>` au lieu de `List<YakadexentryModel>`
- ✅ Utiliser le converter partout

**Avant :**
```java
public List<YakadexentryModel> getAllEntries() {
    return yakadexRepository.findAllEntries();  // ❌ Retourne Models
}

public YakadexentryModel getEntryById(Integer id) {
    return yakadexRepository.findById(id);  // ❌ Retourne Model
}
```

**Après :**
```java
@Inject
ModelConverter modelConverter;  // ✅

public List<YakadexEntryEntity> getAllEntries() {
    List<YakadexentryModel> models = yakadexRepository.findAllEntries();
    return modelConverter.toYakadexEntryEntityList(models);  // ✅
}

public YakadexEntryEntity getEntryById(Integer id) {
    YakadexentryModel model = yakadexRepository.findById(id);
    return modelConverter.toYakadexEntryEntity(model);  // ✅
}
```

---

### 8. **PlayerEntity.java** 🐛 BUG FIX
**Emplacement :** `domain/entity/PlayerEntity.java`

**Changements :**
- 🐛 Corriger `LastFeed` → `lastFeed` (première lettre minuscule)

**Avant :**
```java
private LocalDateTime LastFeed;  // ❌ Majuscule
```

**Après :**
```java
private LocalDateTime lastFeed;  // ✅ Minuscule (camelCase)
```

---

## 📊 RÉSUMÉ DES MODIFICATIONS

| Fichier | Type | Changement principal |
|---------|------|---------------------|
| `GameRepository.java` | Repository | Retourner `GameModel` au lieu de `GameEntity` |
| `PlayerRepository.java` | Repository | Supprimer injection `ModelConverter`, retourner `PlayerModel` |
| `ItemRepository.java` | Repository | ✅ Déjà correct |
| `YakadexEntryRepository.java` | Repository | ✅ Déjà correct |
| `ModelConverter.java` | Converter | Ajouter conversions `Item` et `YakadexEntry` |
| `GameService.java` | Service | Injecter et utiliser `ModelConverter` |
| `PlayerService.java` | Service | Utiliser `ModelConverter` partout |
| `ItemService.java` | Service | Retourner `Entity` et utiliser `ModelConverter` |
| `YakadexEntryService.java` | Service | Retourner `Entity` et utiliser `ModelConverter` |
| `PlayerEntity.java` | Entity | Bug fix : `LastFeed` → `lastFeed` |

---

## 🎯 ORDRE D'APPLICATION RECOMMANDÉ

1. **Corriger PlayerEntity** (bug fix)
2. **Compléter ModelConverter** (ajouter toutes les conversions)
3. **Corriger GameRepository** (enlever conversions)
4. **Corriger PlayerRepository** (enlever injection converter)
5. **Corriger GameService** (utiliser ModelConverter)
6. **Corriger PlayerService** (utiliser ModelConverter)
7. **Corriger ItemService** (utiliser ModelConverter)
8. **Corriger YakadexEntryService** (utiliser ModelConverter)

---

## ✅ VÉRIFICATION FINALE

Après avoir appliqué tous les changements, vérifie que :

- [ ] Aucun Repository n'injecte de Converter
- [ ] Aucun Repository ne fait de conversion Model ↔ Entity
- [ ] Tous les Repositories retournent des Models
- [ ] Tous les Services injectent `ModelConverter`
- [ ] Tous les Services travaillent avec des Entities
- [ ] Tous les Services utilisent le converter pour parler aux Repositories
- [ ] ModelConverter contient toutes les conversions nécessaires

---

## 📝 NOTES IMPORTANTES

### Convention de nommage
- Models : `GameModel`, `PlayerModel`, `ItemModel`, `YakadexentryModel`
- Entities : `GameEntity`, `PlayerEntity`, `ItemEntity`, `YakadexEntryEntity`
- Repositories : `GameRepository`, `PlayerRepository`, etc.
- Services : `GameService`, `PlayerService`, etc.

### Pattern général pour les Services

```java
@ApplicationScoped
public class XxxService {
    
    @Inject
    XxxRepository repository;
    
    @Inject
    ModelConverter modelConverter;
    
    // GET
    public XxxEntity getXxx() {
        XxxModel model = repository.findXxx();
        return modelConverter.toXxxEntity(model);
    }
    
    // SAVE
    @Transactional
    public XxxEntity saveXxx(XxxEntity entity) {
        XxxModel model = modelConverter.toXxxModel(entity);
        XxxModel savedModel = repository.saveXxx(model);
        return modelConverter.toXxxEntity(savedModel);
    }
}
```

### Testing
Après les modifications, lance les tests d'architecture pour vérifier que tout est correct :
```bash
mvn test
```
