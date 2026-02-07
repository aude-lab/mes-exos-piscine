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
                System.out.println("Erreur");
                return;
            }

            System.out.println("Connectee");
            System.out.println();

        } catch (Exception e) {
            System.out.println("Erreur lors de la connexion");
            System.out.println();
        }

        String citizen1Id = initbody.getObject().getString("citizen1Id");
        String citizen2Id = initbody.getObject().getString("citizen2Id");

        System.out.println("Citizens IDs:");
        System.out.println("Citizen 1: " + citizen1Id);
        System.out.println("Citizen 2: " + citizen2Id);
        System.out.println();

        // ========================================
        // ACHIEVEMENT: BUILDER (10 bâtiments)
        // ========================================
        System.out.println("=== OBJECTIF: BUILDER (10 batiments) ===");
        System.out.println();

        // Étape 1: Collecter des ressources (rock pour les routes)
        System.out.println("Etape 1: Collecte de rock pour construire");
        collectRock(baseUrl, myLogin, citizen1Id, 3); // 3 fois pour avoir assez de rock

        // Étape 2: Construire 10 routes (le bâtiment le moins cher)
        System.out.println();
        System.out.println("Etape 2: Construction de 10 batiments (routes)");
        buildTenRoads(baseUrl, myLogin, citizen2Id, 0);

        System.out.println();
        System.out.println("========================================");
        System.out.println("Achievement BUILDER valide!");
        System.out.println("========================================");
    }

    /**
     * Collecte du rock plusieurs fois avec un citizen
     */
    private static void collectRock(String baseUrl, String login, String citizenId, int times) {
        for (int i = 0; i < times; i++) {
            System.out.println("  Collecte " + (i+1) + "/" + times);

            // Observer pour trouver du rock
            String reportId = sendCommand(baseUrl, login, citizenId, "observe");
            if (reportId != null) {
                waitForReport(baseUrl, reportId);
            }

            // Se déplacer vers une tuile de rock (en général il y en a autour)
            // On essaie différentes directions
            String[] directions = {"up", "right", "down", "left"};
            for (String dir : directions) {
                reportId = sendCommand(baseUrl, login, citizenId, "move:" + dir);
                if (reportId != null) {
                    waitForReport(baseUrl, reportId);

                    // Essayer de collecter
                    reportId = sendCommand(baseUrl, login, citizenId, "gather");
                    if (reportId != null) {
                        waitForReport(baseUrl, reportId);
                        System.out.println("    -> Rock collecte!");
                        break; // On a collecté, on sort de la boucle
                    }
                }
            }

            // Retourner au town hall pour décharger
            // Le town hall est généralement proche, on fait quelques mouvements
            for (int j = 0; j < 2; j++) {
                reportId = sendCommand(baseUrl, login, citizenId, "move:down");
                if (reportId != null) waitForReport(baseUrl, reportId);
            }

            // Décharger les ressources
            reportId = sendCommand(baseUrl, login, citizenId, "unload");
            if (reportId != null) {
                waitForReport(baseUrl, reportId);
                System.out.println("    -> Ressources dechargees au town hall");
            }
        }
    }

    /**
     * Construit 10 routes avec un citizen
     */
    private static void buildTenRoads(String baseUrl, String login, String citizenId, int buildingsBuilt) {
        int targetBuildings = 10;

        // Observer d'abord
        String reportId = sendCommand(baseUrl, login, citizenId, "observe");
        if (reportId != null) waitForReport(baseUrl, reportId);

        // Se déplacer et construire 10 fois
        String[] directions = {"up", "right", "down", "left"};
        int dirIndex = 0;

        while (buildingsBuilt < targetBuildings) {
            // Se déplacer vers une nouvelle position
            String direction = directions[dirIndex % 4];
            reportId = sendCommand(baseUrl, login, citizenId, "move:" + direction);
            if (reportId != null) waitForReport(baseUrl, reportId);

            // Construire une route
            reportId = sendCommand(baseUrl, login, citizenId, "build:road");
            if (reportId != null) {
                waitForReport(baseUrl, reportId);
                buildingsBuilt++;
                System.out.println("  Batiment " + buildingsBuilt + "/" + targetBuildings + " construit (road)");
            }

            dirIndex++;

            // Petit délai pour éviter de surcharger
            try {
                Thread.sleep(300);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
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

                // Petit délai entre chaque tentative
                Thread.sleep(50);
            } catch (Exception e){
                return;
            }

        }
    }


}