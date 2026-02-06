package com.epita.creeps;

import com.epita.given.Cartographer;
import com.epita.given.Json;
import com.epita.given.response.InitResponse;
import kong.unirest.Unirest;
import kong.unirest.HttpResponse;
import kong.unirest.JsonNode;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Point d'entrée principal du programme Creeps.
 * 
 * Ce programme doit être lancé avec 3 arguments :
 * 1. hostname : l'adresse du serveur (ex: localhost)
 * 2. port : le port du serveur (ex: 1664)
 * 3. login : votre login unique (ex: prenom.nom)
 * 
 * Exemple d'exécution :
 * java -jar target/creeps-1.0.0-SNAPSHOT.jar localhost 1664 ton.login
 */
public class Program {
    
    private static final Logger logger = LoggerFactory.getLogger(Program.class);
    
    private String baseUrl;
    private String login;
    private InitResponse initResponse;
    
    public Program(String hostname, int port, String login) {
        this.baseUrl = "http://" + hostname + ":" + port;
        this.login = login;
    }
    
    /**
     * Point d'entrée du programme
     */
    public static void main(String[] args) {
        // Vérification des arguments
        if (args.length != 3) {
            System.err.println("❌ Erreur : Nombre d'arguments incorrect");
            System.err.println("Usage: java -jar creeps.jar <hostname> <port> <login>");
            System.err.println("Exemple: java -jar creeps.jar localhost 1664 prenom.nom");
            System.exit(1);
        }
        
        try {
            String hostname = args[0];
            int port = Integer.parseInt(args[1]);
            String login = args[2];
            
            logger.info("🚀 Démarrage du programme Creeps");
            logger.info("📍 Serveur: {}:{}", hostname, port);
            logger.info("👤 Login: {}", login);
            
            // Créer et lancer le programme
            Program program = new Program(hostname, port, login);
            program.run();
            
        } catch (NumberFormatException e) {
            System.err.println("❌ Erreur : Le port doit être un nombre");
            System.exit(1);
        } catch (Exception e) {
            logger.error("❌ Erreur fatale", e);
            System.exit(1);
        }
    }
    
    /**
     * Méthode principale qui lance le jeu
     */
    public void run() {
        try {
            // Étape 1 : Vérifier que le serveur est en ligne
            if (!checkServerStatus()) {
                logger.error("❌ Le serveur n'est pas accessible");
                return;
            }
            
            // Étape 2 : Se connecter au jeu
            if (!connect()) {
                logger.error("❌ Échec de la connexion au jeu");
                return;
            }
            
            // Étape 3 : Afficher les informations de départ
            displayInitInfo();
            
            // Étape 4 : Lancer la logique du jeu
            logger.info("🎮 Lancement de la logique du jeu...");
            startGame();
            
            logger.info("✅ Programme terminé");
            
        } catch (Exception e) {
            logger.error("❌ Erreur dans la boucle principale", e);
        }
    }
    
    /**
     * Vérifie que le serveur est accessible et en cours d'exécution
     */
    private boolean checkServerStatus() {
        try {
            logger.info("🔍 Vérification du statut du serveur...");
            
            HttpResponse<JsonNode> response = Unirest.get(baseUrl + "/status")
                .asJson();
            
            if (response.getStatus() == 200) {
                JsonNode body = response.getBody();
                boolean running = body.getObject().getBoolean("running");
                
                if (running) {
                    logger.info("✅ Serveur accessible et en cours d'exécution");
                    return true;
                } else {
                    logger.warn("⚠️ Serveur accessible mais le jeu n'est pas démarré");
                    return false;
                }
            } else {
                logger.error("❌ Erreur HTTP: {}", response.getStatus());
                return false;
            }
            
        } catch (Exception e) {
            logger.error("❌ Impossible de contacter le serveur", e);
            return false;
        }
    }
    
    /**
     * Se connecte au jeu et récupère les informations initiales
     */
    private boolean connect() {
        try {
            logger.info("🔌 Connexion au jeu avec le login: {}", login);
            
            HttpResponse<String> response = Unirest.post(baseUrl + "/init/" + login)
                .asString();
            
            if (response.getStatus() == 200) {
                // Parser la réponse avec la bibliothèque Given
                initResponse = Json.parse(response.getBody(), InitResponse.class);
                
                // Vérifier s'il y a une erreur
                if (initResponse.getError() != null) {
                    logger.error("❌ Erreur lors de la connexion: {}", initResponse.getError());
                    return false;
                }
                
                logger.info("✅ Connexion réussie !");
                logger.info("🎉 Achievement débloqué: Hello, World!");
                return true;
                
            } else {
                logger.error("❌ Erreur HTTP lors de la connexion: {}", response.getStatus());
                return false;
            }
            
        } catch (Exception e) {
            logger.error("❌ Erreur lors de la connexion", e);
            return false;
        }
    }
    
    /**
     * Affiche les informations reçues lors de l'initialisation
     */
    private void displayInitInfo() {
        logger.info("═══════════════════════════════════════════════");
        logger.info("📊 INFORMATIONS DE DÉPART");
        logger.info("═══════════════════════════════════════════════");
        
        // Afficher la position du Town Hall
        logger.info("🏛️  Town Hall: {}", initResponse.getTownHallPosition());
        
        // Afficher la position du Household
        logger.info("🏠 Household: {}", initResponse.getHouseholdPosition());
        
        // Afficher les IDs des citizens
        logger.info("👥 Citizen 1 ID: {}", initResponse.getCitizen1Id());
        logger.info("👥 Citizen 2 ID: {}", initResponse.getCitizen2Id());
        
        // Afficher les ressources de départ
        logger.info("💰 Ressources de départ:");
        logger.info("   - Wood: {}", initResponse.getStartingResources().getWood());
        logger.info("   - Rock: {}", initResponse.getStartingResources().getRock());
        logger.info("   - Food: {}", initResponse.getStartingResources().getFood());
        logger.info("   - Oil: {}", initResponse.getStartingResources().getOil());
        
        // Afficher quelques infos sur la configuration
        logger.info("⚙️  Configuration:");
        logger.info("   - Ticks par seconde: {}", initResponse.getConfig().getTicksPerSecond());
        logger.info("   - Hector activé: {}", initResponse.getConfig().isEnableGC());
        logger.info("   - Ennemis activés: {}", initResponse.getConfig().isEnableEnemies());
        
        logger.info("═══════════════════════════════════════════════");
    }
    
    /**
     * Lance la logique principale du jeu
     * 
     * TODO: C'est ici que vous devez implémenter votre IA !
     */
    private void startGame() {
        logger.info("🎯 Démarrage de la logique du jeu");
        
        // TODO: Implémenter votre logique ici
        // Quelques suggestions pour commencer :
        
        // 1. Créer des objets pour gérer vos unités
        //    - Créer une classe Citizen qui wrap l'ID et peut exécuter des commandes
        //    - Créer une classe pour gérer la file d'attente des commandes
        
        // 2. Envoyer une première commande simple (par exemple, observer)
        //    String citizen1Id = initResponse.getCitizen1Id();
        //    sendCommand(citizen1Id, "observe");
        
        // 3. Créer des threads pour chaque unité
        //    - Chaque unité peut avoir son propre thread
        //    - Utiliser BlockingQueue pour gérer les commandes
        
        // 4. Implémenter une boucle de jeu
        //    - Observer l'environnement
        //    - Récolter des ressources
        //    - Construire des bâtiments
        //    - Se défendre contre les ennemis
        
        logger.warn("⚠️  La logique du jeu n'est pas encore implémentée !");
        logger.info("💡 Conseil: Commencez par envoyer une commande simple comme 'observe'");
        
        // Exemple basique : envoyer une commande observe au premier citizen
        try {
            logger.info("📡 Envoi d'une commande 'observe' pour tester...");
            String citizen1Id = initResponse.getCitizen1Id();
            String reportId = sendCommand(citizen1Id, "observe");
            
            if (reportId != null) {
                logger.info("✅ Commande envoyée ! Report ID: {}", reportId);
                logger.info("🎉 Achievement débloqué: Let's go!");
                
                // Attendre un peu pour que la commande soit exécutée
                Thread.sleep(2000);
                
                // Récupérer le rapport
                String report = getReport(reportId);
                if (report != null) {
                    logger.info("📋 Rapport reçu: {}", report);
                }
            }
            
        } catch (Exception e) {
            logger.error("❌ Erreur lors de l'envoi de la commande de test", e);
        }
    }
    
    /**
     * Envoie une commande à une unité
     * 
     * @param unitId L'ID de l'unité
     * @param opcode Le code de l'opération (ex: "observe", "move:up", etc.)
     * @return Le report ID si la commande est acceptée, null sinon
     */
    private String sendCommand(String unitId, String opcode) {
        return sendCommand(unitId, opcode, null);
    }
    
    /**
     * Envoie une commande à une unité avec un body optionnel
     * 
     * @param unitId L'ID de l'unité
     * @param opcode Le code de l'opération
     * @param body Le corps de la requête (peut être null)
     * @return Le report ID si la commande est acceptée, null sinon
     */
    private String sendCommand(String unitId, String opcode, String body) {
        try {
            String url = String.format("%s/command/%s/%s/%s", baseUrl, login, unitId, opcode);
            
            var request = Unirest.post(url);
            
            // Ajouter le body si présent
            if (body != null && !body.isEmpty()) {
                request.body(body);
            }
            
            HttpResponse<JsonNode> response = request.asJson();
            
            if (response.getStatus() == 200) {
                JsonNode responseBody = response.getBody();
                String responseOpcode = responseBody.getObject().optString("opcode", "");
                
                // Vérifier si c'est une erreur
                if (responseOpcode.equals("notrunning") || 
                    responseOpcode.equals("unrecognized") || 
                    responseOpcode.equals("unavailable") ||
                    responseBody.getObject().has("errorCode")) {
                    
                    String error = responseBody.getObject().optString("error", "Erreur inconnue");
                    logger.error("❌ Erreur commande: {}", error);
                    return null;
                }
                
                // Récupérer le report ID
                String reportId = responseBody.getObject().optString("reportId", null);
                return reportId;
                
            } else {
                logger.error("❌ Erreur HTTP lors de l'envoi de la commande: {}", response.getStatus());
                return null;
            }
            
        } catch (Exception e) {
            logger.error("❌ Erreur lors de l'envoi de la commande", e);
            return null;
        }
    }
    
    /**
     * Récupère un rapport d'exécution de commande
     * 
     * @param reportId L'ID du rapport
     * @return Le rapport en JSON, ou null si non disponible
     */
    private String getReport(String reportId) {
        try {
            String url = String.format("%s/report/%s", baseUrl, reportId);
            
            HttpResponse<String> response = Unirest.get(url).asString();
            
            if (response.getStatus() == 200) {
                String body = response.getBody();
                
                // Vérifier si c'est une erreur "noreport"
                if (body.contains("\"opcode\":\"noreport\"")) {
                    logger.warn("⚠️  Rapport pas encore prêt: {}", reportId);
                    return null;
                }
                
                return body;
                
            } else {
                logger.error("❌ Erreur HTTP lors de la récupération du rapport: {}", response.getStatus());
                return null;
            }
            
        } catch (Exception e) {
            logger.error("❌ Erreur lors de la récupération du rapport", e);
            return null;
        }
    }
    
    /**
     * Getter pour l'InitResponse (utile pour d'autres classes)
     */
    public InitResponse getInitResponse() {
        return initResponse;
    }
    
    /**
     * Getter pour le login
     */
    public String getLogin() {
        return login;
    }
    
    /**
     * Getter pour l'URL de base
     */
    public String getBaseUrl() {
        return baseUrl;
    }
}
