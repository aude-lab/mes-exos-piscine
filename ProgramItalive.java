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
        System.out.println("Vérification cotee serveur:");
        HttpResponse<JsonNode> statusResponse;
        try {
            statusResponse = Unirest.get(baseUrl + "/status")
                    .asJson();

            boolean running = statusResponse.getBody().getObject().getBoolean("running");

            if (running) {
                System.out.println("Serveur en ligne");
                System.out.println();
            } else {
                System.out.println();
                System.out.println("Le jeu n'est pas démarré");
                System.out.println();
                return;
            }
        } catch (Exception e) {
            System.out.println("Impossible de contacter le serveur");
            return;
        }

        System.out.println("Connexion au jeu:");
        JsonNode initbody = null;
        try {
            HttpResponse<JsonNode> initResponse = Unirest.post(baseUrl + "/init/" + myLogin)
                    .asJson();
            initbody = initResponse.getBody();
            if (initbody.getObject().has("error") && !initbody.getObject().isNull("error")) {
                System.out.println("Erreur d'initialisation: " + initbody.getObject().getString("error"));
                return;
            }

            System.out.println("Connectee");
            System.out.println();

        } catch (Exception e) {
            System.out.println("Erreur lors de la connexion: " + e.getMessage());
            return;
        }

        if (initbody == null) {
            System.out.println("Erreur: Impossible de se connecter");
            return;
        }

        String citizen1Id = initbody.getObject().getString("citizen1Id");
        String citizen2Id = initbody.getObject().getString("citizen2Id");

        System.out.println("Citizens IDs:");
        System.out.println("Citizen 1: " + citizen1Id);
        System.out.println("Citizen 2: " + citizen2Id);
        System.out.println();

        // ========================================
        // ACHIEVEMENT: IT'S ALIIIIIVVVVE! (spawn unité mécanique)
        // ========================================
        System.out.println("=== OBJECTIF: IT'S ALIIIIIVVVVE! ===");
        System.out.println();
        System.out.println("Au debut du jeu, tu as 30 rock/wood/food");
        System.out.println("Un turret coute 5 rock");
        System.out.println();

        // Spawn d'un turret avec citizen1
        System.out.println("Spawn d'un turret avec citizen1...");
        String reportId = sendCommand(baseUrl, myLogin, citizen1Id, "spawn:turret");
        if (reportId != null) {
            waitForReport(baseUrl, reportId);
            System.out.println("SUCCESS: Turret spawne!");
        } else {
            System.out.println("ECHEC: Pas assez de ressources ou erreur");
        }

        System.out.println();
        System.out.println("========================================");
        System.out.println("Achievement IT'S ALIIIIIVVVVE! devrait etre valide!");
        System.out.println("========================================");
    }

    private static String sendCommand(String baseUrl, String login, String unitId, String opcode) {
        try {
            String url = baseUrl + "/command/" + login + "/" + unitId + "/" + opcode;

            HttpResponse<JsonNode> response = Unirest.post(url)
                    .body("{}")
                    .asJson();

            JsonNode body = response.getBody();

            if (body.getObject().has("errorCode") && !body.getObject().isNull("errorCode")) {
                String errorCode = body.getObject().getString("errorCode");
                System.out.println("Error: " + errorCode);
                if (body.getObject().has("error")) {
                    System.out.println("Details: " + body.getObject().getString("error"));
                }
                return null;
            }

            String reportId = body.getObject().getString("reportId");
            return reportId;

        } catch (Exception e) {
            System.out.println("Erreur lors de l'envoie de la commande: " + e.getMessage());
            return null;
        }
    }

    private static void waitForReport(String baseUrl, String reportId) {
        int attempts = 0;
        int maxattempts = 1000;
        while (attempts < maxattempts) {
            attempts++;

            try {
                String url = baseUrl + "/report/" + reportId;
                HttpResponse<JsonNode> response = Unirest.get(url).asJson();
                JsonNode body = response.getBody();
                String opcode = body.getObject().getString("opcode");
                if (!opcode.equals("noreport")){
                    return;
                }
            } catch (Exception e){
                return;
            }

        }
    }


}