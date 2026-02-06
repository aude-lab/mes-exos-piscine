package com.epita.creeps;

import kong.unirest.core.Unirest;
import kong.unirest.core.HttpResponse;
import kong.unirest.core.JsonNode;

public class Program {
    public static void main(String args[]) {
        if (args.length != 3) {
            System.err.println("args number error ");
            System.exit(1);
        }
        String host = args[0];
        int port = Integer.parseInt(args[1]);
        String myLogin = args[2];
        System.out.println("Creeps start:");
        System.out.println("Serveur: " + host + ":" + port);
        System.out.println("Login: " + myLogin);
        System.out.println();
        String baseUrl = "http://" + host + ":" + port;
        System.out.println("Verification cotee serveur:");
        try {
            HttpResponse<JsonNode> statusResponse = Unirest.get(baseUrl + "/status")
                    .asJson();

            boolean running = statusResponse.getBody().getObject().getBoolean("running");

            if (running) {
                System.out.println("Serveur en ligne");
                System.out.println();
            } else {
                System.out.println();
                System.out.println("Le jeu n'est pas demarre");
                System.out.println();
                return;
            }
        } catch (Exception e) {
            System.out.println("Impossible de contacter le serveur");
            return;
        }

        System.out.println("Connexion au jeu:");
        JsonNode initBody = null;
        try {
            HttpResponse<JsonNode> initResponse = Unirest.post(baseUrl + "/init/" + myLogin)
                    .asJson();
            initBody = initResponse.getBody();
            if (initBody.getObject().has("error") && !initBody.getObject().isNull("error")) {
                System.out.println("Erreur: " + initBody.getObject().getString("error"));
                return;
            }

            System.out.println("Connectee");
            System.out.println("Achievement: Hello, World!");
            System.out.println();

        } catch (Exception e) {
            System.out.println("Erreur lors de la connexion");
            System.out.println();
            return;
        }

        // Récupérer les IDs des citizens
        String citizen1Id = initBody.getObject().getString("citizen1Id");
        String citizen2Id = initBody.getObject().getString("citizen2Id");
        
        System.out.println("Citizens IDs:");
        System.out.println("  Citizen 1: " + citizen1Id);
        System.out.println("  Citizen 2: " + citizen2Id);
        System.out.println();

        // ========================================
        // COMMANDE 1 : Observer avec citizen 1
        // ========================================
        System.out.println("=== Commande 1: Observer ===");
        String reportId1 = sendCommand(baseUrl, myLogin, citizen1Id, "observe");
        if (reportId1 != null) {
            System.out.println("Commande observe envoyee !");
            System.out.println("Achievement: Let's go!");
            System.out.println("Report ID: " + reportId1);
            System.out.println();
            
            // Attendre que la commande soit executee (1 tick = ~1 seconde en mode normal)
            waitSeconds(2);
            
            // Recuperer le rapport
            getReport(baseUrl, reportId1);
        }

        // ========================================
        // COMMANDE 2 : Bouger citizen 1 vers le haut
        // ========================================
        System.out.println("=== Commande 2: Bouger vers le haut ===");
        String reportId2 = sendCommand(baseUrl, myLogin, citizen1Id, "move:up");
        if (reportId2 != null) {
            System.out.println("Commande move:up envoyee !");
            System.out.println("Achievement: Oscar Mike!");
            System.out.println("Report ID: " + reportId2);
            System.out.println();
            
            waitSeconds(3);
            getReport(baseUrl, reportId2);
        }

        // ========================================
        // COMMANDE 3 : Observer avec citizen 2
        // ========================================
        System.out.println("=== Commande 3: Observer avec citizen 2 ===");
        String reportId3 = sendCommand(baseUrl, myLogin, citizen2Id, "observe");
        if (reportId3 != null) {
            System.out.println("Commande observe envoyee !");
            System.out.println("Report ID: " + reportId3);
            System.out.println();
            
            waitSeconds(2);
            getReport(baseUrl, reportId3);
        }

        System.out.println("=== Programme termine ===");
        System.out.println("Va voir le viewer pour verifier les achievements !");
        System.out.println("http://localhost:1337");
    }

    /**
     * Envoie une commande a une unite
     */
    private static String sendCommand(String baseUrl, String login, String unitId, String opcode) {
        try {
            String url = baseUrl + "/command/" + login + "/" + unitId + "/" + opcode;
            System.out.println("Envoi: POST " + url);
            
            HttpResponse<JsonNode> response = Unirest.post(url)
                    .body("{}")
                    .asJson();

            JsonNode body = response.getBody();
            
            // Verifier les erreurs
            if (body.getObject().has("errorCode") && !body.getObject().isNull("errorCode")) {
                String errorCode = body.getObject().getString("errorCode");
                System.out.println("ERREUR: " + errorCode);
                if (body.getObject().has("error")) {
                    System.out.println("Details: " + body.getObject().getString("error"));
                }
                return null;
            }

            // Recuperer le report ID
            String reportId = body.getObject().getString("reportId");
            return reportId;

        } catch (Exception e) {
            System.out.println("Erreur lors de l'envoi de la commande: " + e.getMessage());
            return null;
        }
    }

    /**
     * Recupere un rapport de commande
     */
    private static void getReport(String baseUrl, String reportId) {
        try {
            String url = baseUrl + "/report/" + reportId;
            System.out.println("Recuperation du rapport: " + reportId);
            
            HttpResponse<JsonNode> response = Unirest.get(url).asJson();
            JsonNode body = response.getBody();
            
            // Verifier si le rapport est pret
            String opcode = body.getObject().getString("opcode");
            if (opcode.equals("noreport")) {
                System.out.println("Rapport pas encore pret");
                System.out.println();
                return;
            }

            // Afficher le statut
            String status = body.getObject().getString("status");
            System.out.println("Statut: " + status);
            
            if (status.equals("SUCCESS")) {
                System.out.println("Commande executee avec succes !");
                
                // Afficher la position si presente
                if (body.getObject().has("newPosition")) {
                    JsonNode newPos = body.getObject().getJSONObject("newPosition");
                    System.out.println("Nouvelle position: x=" + newPos.getObject().getInt("x") + 
                                     ", y=" + newPos.getObject().getInt("y"));
                }
            } else {
                System.out.println("Commande echouee !");
                if (body.getObject().has("errorCode")) {
                    System.out.println("Code erreur: " + body.getObject().getString("errorCode"));
                }
            }
            System.out.println();

        } catch (Exception e) {
            System.out.println("Erreur lors de la recuperation du rapport: " + e.getMessage());
            System.out.println();
        }
    }

    /**
     * Attendre quelques secondes
     */
    private static void waitSeconds(int seconds) {
        try {
            System.out.println("Attente de " + seconds + " secondes...");
            Thread.sleep(seconds * 1000);
        } catch (InterruptedException e) {
            // Ignorer
        }
    }
}

