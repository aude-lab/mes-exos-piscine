package com.epita.creeps;

import kong.unirest.Unirest;
import kong.unirest.HttpResponse;
import kong.unirest.JsonNode;

public class Program {
    public static void main(String[] args) {
        if (args.length != 3) {
            System.err.println("Usage: java -jar creeps.jar <hostname> <port> <login>");
            System.exit(1);
        }
        
        String hostname = args[0];
        int port = Integer.parseInt(args[1]);
        String login = args[2];
        
        System.out.println("==============================================");
        System.out.println("🎮 CREEPS - Démarrage");
        System.out.println("==============================================");
        System.out.println("📍 Serveur: " + hostname + ":" + port);
        System.out.println("👤 Login: " + login);
        System.out.println();
        
        // Construire l'URL de base
        String baseUrl = "http://" + hostname + ":" + port;
        
        // Étape 1 : Vérifier que le serveur est en ligne
        System.out.println("🔍 Vérification du serveur...");
        try {
            HttpResponse<JsonNode> statusResponse = Unirest.get(baseUrl + "/status")
                .asJson();
            
            boolean running = statusResponse.getBody().getObject().getBoolean("running");
            
            if (running) {
                System.out.println("✅ Serveur en ligne !");
            } else {
                System.out.println("❌ Le jeu n'est pas démarré");
                return;
            }
        } catch (Exception e) {
            System.out.println("❌ Impossible de contacter le serveur");
            e.printStackTrace();
            return;
        }
        
        System.out.println();
        
        // Étape 2 : Se connecter au jeu
        System.out.println("🔌 Connexion au jeu...");
        try {
            HttpResponse<JsonNode> initResponse = Unirest.post(baseUrl + "/init/" + login)
                .asJson();
            
            JsonNode body = initResponse.getBody();
            
            // Vérifier s'il y a une erreur
            if (body.getObject().has("error") && !body.getObject().isNull("error")) {
                System.out.println("❌ Erreur: " + body.getObject().getString("error"));
                return;
            }
            
            System.out.println("✅ Connexion réussie !");
            System.out.println("🎉 Achievement débloqué: Hello, World!");
            System.out.println();
            
            // Afficher les informations reçues
            System.out.println("==============================================");
            System.out.println("📊 INFORMATIONS DE DÉPART");
            System.out.println("==============================================");
            
            // Position du Town Hall
            JsonNode townHallPos = body.getObject().getJSONObject("townHallPosition");
            System.out.println("🏛️  Town Hall:");
            System.out.println("   Position: x=" + townHallPos.getObject().getInt("x") + 
                             ", y=" + townHallPos.getObject().getInt("y"));
            
            // Position du Household
            JsonNode householdPos = body.getObject().getJSONObject("householdPosition");
            System.out.println("🏠 Household:");
            System.out.println("   Position: x=" + householdPos.getObject().getInt("x") + 
                             ", y=" + householdPos.getObject().getInt("y"));
            
            // IDs des citizens
            String citizen1Id = body.getObject().getString("citizen1Id");
            String citizen2Id = body.getObject().getString("citizen2Id");
            System.out.println("👥 Citizens:");
            System.out.println("   Citizen 1 ID: " + citizen1Id);
            System.out.println("   Citizen 2 ID: " + citizen2Id);
            
            // Ressources de départ
            JsonNode resources = body.getObject().getJSONObject("startingResources");
            System.out.println("💰 Ressources:");
            System.out.println("   Wood: " + resources.getObject().getInt("wood"));
            System.out.println("   Rock: " + resources.getObject().getInt("rock"));
            System.out.println("   Food: " + resources.getObject().getInt("food"));
            System.out.println("   Oil: " + resources.getObject().getInt("oil"));
            
            System.out.println("==============================================");
            System.out.println();
            System.out.println("✨ Tu es maintenant connecté au jeu !");
            System.out.println("💡 Prochaine étape: Envoyer des commandes aux citizens");
            
        } catch (Exception e) {
            System.out.println("❌ Erreur lors de la connexion");
            e.printStackTrace();
        }
    }
}
