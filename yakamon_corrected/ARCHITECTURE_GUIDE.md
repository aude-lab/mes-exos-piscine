# 🏗️ ARCHITECTURE EN COUCHES - GUIDE COMPLET

## 📊 Vue d'ensemble

L'architecture en couches sépare les responsabilités en 3 couches principales :

```
┌─────────────────────────────────────────────┐
│     PRESENTATION LAYER (Controllers)        │
│  - Gère les requêtes HTTP                   │
│  - Valide les inputs                        │
│  - Travaille avec DTOs                      │
└──────────────┬──────────────────────────────┘
               │ DTOs ↓ / ↑ DTOs
               │ (via GameConverter)
┌──────────────▼──────────────────────────────┐
│      BUSINESS LAYER (Services)              │
│  - Contient la logique métier               │
│  - Travaille avec Entities                  │
│  - Utilise ModelConverter pour Data Layer   │
└──────────────┬──────────────────────────────┘
               │ Entities ↓ / ↑ Models
               │ (via ModelConverter)
┌──────────────▼──────────────────────────────┐
│       DATA LAYER (Repositories)             │
│  - Gère les opérations DB                   │
│  - Travaille UNIQUEMENT avec Models         │
│  - Ne fait AUCUNE conversion                │
└─────────────────────────────────────────────┘
```

## ✅ RÈGLES ABSOLUES

### 1. REPOSITORIES (Data Layer)
❌ **NE DOIT PAS :**
- Retourner des Entities
- Injecter des Converters
- Faire des conversions Model ↔ Entity
- Contenir de la logique métier

✅ **DOIT :**
- Retourner des Models
- Prendre des Models en paramètre
- Uniquement gérer les opérations DB

**Exemple :**
```java
@ApplicationScoped
public class GameRepository implements PanacheRepositoryBase<GameModel, Integer> {
    
    // ✅ CORRECT : Retourne un Model
    public GameModel findGame() {
        return listAll().stream().findFirst().orElse(null);
    }
    
    // ✅ CORRECT : Prend et retourne un Model
    public GameModel saveGame(GameModel model) {
        persist(model);
        return model;
    }
    
    // ❌ INCORRECT : Ne devrait PAS retourner une Entity
    public GameEntity findGame() {
        GameModel model = listAll().stream().findFirst().orElse(null);
        return toEntity(model);  // ❌ Conversion dans le Repository
    }
}
```

### 2. CONVERTERS (Model ↔ Entity)
✅ **DOIT :**
- Faire les conversions Model → Entity
- Faire les conversions Entity → Model
- Être injecté dans les Services
- Ne contenir AUCUNE logique métier

**Exemple :**
```java
@ApplicationScoped
public class ModelConverter {
    
    // ✅ CORRECT : Conversion Model → Entity
    public GameEntity toGameEntity(GameModel model) {
        if (model == null) return null;
        
        GameEntity entity = new GameEntity();
        entity.setId(model.getId());
        entity.setMap(model.getMap());
        return entity;
    }
    
    // ✅ CORRECT : Conversion Entity → Model
    public GameModel toGameModel(GameEntity entity) {
        if (entity == null) return null;
        
        GameModel model = new GameModel();
        model.setId(entity.getId());
        model.setMap(entity.getMap());
        return model;
    }
}
```

### 3. SERVICES (Business Layer)
✅ **DOIT :**
- Travailler avec Entities (pas Models)
- Injecter ModelConverter
- Utiliser le converter pour communiquer avec les Repositories
- Contenir toute la logique métier

**Exemple :**
```java
@ApplicationScoped
public class GameService {
    
    @Inject
    GameRepository gameRepository;
    
    @Inject
    ModelConverter modelConverter;  // ✅ Injecte le converter
    
    // ✅ CORRECT : Utilise le converter
    public GameEntity getGame() {
        GameModel model = gameRepository.findGame();  // Repository retourne Model
        return modelConverter.toGameEntity(model);     // Converter transforme en Entity
    }
    
    @Transactional
    public GameEntity createGame(String mapFilePath) {
        // 1. Travaille avec Entity
        GameEntity entity = new GameEntity();
        entity.setMap(parsedMap);
        
        // 2. Convertit Entity → Model pour le Repository
        GameModel model = modelConverter.toGameModel(entity);
        GameModel savedModel = gameRepository.saveGame(model);
        
        // 3. Reconvertit Model → Entity pour le retour
        return modelConverter.toGameEntity(savedModel);
    }
}
```

### 4. CONTROLLERS (Presentation Layer)
✅ **DOIT :**
- Travailler avec DTOs (Request/Response)
- Injecter GameConverter (pour Entity → DTO)
- Appeler les Services avec des primitives ou des Entities
- Valider les inputs

**Exemple :**
```java
@Path("/start")
public class GameResource {
    
    @Inject
    GameService gameService;
    
    @Inject
    GameConverter gameConverter;  // ✅ Converter pour Entity → DTO
    
    @POST
    public Response startGame(StartRequest request) {
        // 1. Validation
        if (request.getPlayerName() == null) {
            return Response.status(400).entity(new ErrorInfo("Invalid name")).build();
        }
        
        // 2. Appel du Service (retourne une Entity)
        GameEntity game = gameService.startNewGame(
            request.getPlayerName(),
            request.getMapPath()
        );
        
        // 3. Conversion Entity → DTO pour la réponse
        StartResponse response = gameConverter.toStartResponse(game);
        return Response.ok(response).build();
    }
}
```

## 🔄 FLUX COMPLET (Exemple : START endpoint)

### Request → Response

```
1. CLIENT envoie StartRequest (DTO)
   ↓
2. CONTROLLER (GameResource)
   - Reçoit StartRequest (DTO)
   - Valide les données
   - Appelle gameService.startNewGame(name, path)
   ↓
3. SERVICE (GameService)
   - Crée une GameEntity
   - Convertit GameEntity → GameModel via modelConverter
   - Appelle gameRepository.saveGame(model)
   ↓
4. REPOSITORY (GameRepository)
   - Reçoit GameModel
   - Sauvegarde dans la DB avec persist(model)
   - Retourne GameModel
   ↓
5. SERVICE (GameService)
   - Reçoit GameModel du Repository
   - Convertit GameModel → GameEntity via modelConverter
   - Retourne GameEntity
   ↓
6. CONTROLLER (GameResource)
   - Reçoit GameEntity du Service
   - Convertit GameEntity → StartResponse via gameConverter
   - Retourne Response avec StartResponse (DTO)
   ↓
7. CLIENT reçoit StartResponse (DTO)
```

## 🔍 COMPARAISON AVANT/APRÈS

### ❌ AVANT (Incorrect)

**GameRepository.java**
```java
public class GameRepository {
    // ❌ Fait la conversion dans le Repository
    public GameEntity findGame() {
        GameModel model = listAll().stream().findFirst().orElse(null);
        return toEntity(model);  // ❌ Conversion ici
    }
    
    private GameEntity toEntity(GameModel model) { ... }  // ❌ Conversion dans Repository
}
```

**GameService.java**
```java
public class GameService {
    @Inject
    GameRepository gameRepository;
    
    // ❌ Reçoit directement une Entity du Repository
    public GameEntity getGame() {
        return gameRepository.findGame();  // ❌ Repository retourne une Entity
    }
}
```

### ✅ APRÈS (Correct)

**GameRepository.java**
```java
public class GameRepository {
    // ✅ Retourne un Model
    public GameModel findGame() {
        return listAll().stream().findFirst().orElse(null);
    }
}
```

**ModelConverter.java**
```java
@ApplicationScoped
public class ModelConverter {
    // ✅ Conversion dans le Converter
    public GameEntity toGameEntity(GameModel model) {
        if (model == null) return null;
        GameEntity entity = new GameEntity();
        entity.setId(model.getId());
        entity.setMap(model.getMap());
        return entity;
    }
}
```

**GameService.java**
```java
public class GameService {
    @Inject
    GameRepository gameRepository;
    
    @Inject
    ModelConverter modelConverter;  // ✅ Injecte le converter
    
    // ✅ Utilise le converter pour transformer Model → Entity
    public GameEntity getGame() {
        GameModel model = gameRepository.findGame();
        return modelConverter.toGameEntity(model);
    }
}
```

## 📝 CHECKLIST DE VÉRIFICATION

### Pour chaque Repository :
- [ ] Ne retourne QUE des Models
- [ ] Ne prend QUE des Models en paramètre
- [ ] N'injecte AUCUN Converter
- [ ] Ne contient AUCUNE méthode de conversion
- [ ] Ne contient AUCUNE logique métier

### Pour chaque Service :
- [ ] Travaille avec des Entities (pas Models)
- [ ] Injecte ModelConverter
- [ ] Utilise le converter avant d'appeler le Repository
- [ ] Utilise le converter après avoir reçu du Repository
- [ ] Contient la logique métier

### Pour chaque Controller :
- [ ] Travaille avec des DTOs (Request/Response)
- [ ] Injecte le bon Converter (GameConverter, etc.)
- [ ] Valide les inputs
- [ ] Convertit Entity → DTO avant de retourner

## 🎯 RÉSUMÉ

```
Repository    →  Travaille avec Models uniquement
                 Ne fait AUCUNE conversion

ModelConverter → Convertit Model ↔ Entity
                 Injecté dans les Services

Service       →  Travaille avec Entities
                 Utilise ModelConverter pour parler au Repository

GameConverter →  Convertit Entity ↔ DTO
                 Injecté dans les Controllers

Controller    →  Travaille avec DTOs
                 Utilise GameConverter pour parler au Service
```

## 🚨 ERREURS COURANTES

1. **Repository qui retourne des Entities**
   - ❌ `public GameEntity findGame()`
   - ✅ `public GameModel findGame()`

2. **Repository qui injecte un Converter**
   - ❌ `@Inject ModelConverter converter;`
   - ✅ Pas de converter dans les Repositories

3. **Service qui travaille avec des Models**
   - ❌ `List<ItemModel> getAllItems()`
   - ✅ `List<ItemEntity> getAllItems()`

4. **Service qui n'utilise pas le Converter**
   - ❌ `return gameRepository.findGame();` (retourne Model)
   - ✅ `return modelConverter.toGameEntity(gameRepository.findGame());`

5. **Conversion dans le Repository**
   - ❌ `private GameEntity toEntity(GameModel m) { ... }`
   - ✅ Déplacer dans ModelConverter
