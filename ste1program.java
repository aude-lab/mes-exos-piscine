package com.epita.creeps;

public class Program {
    public static void main(String[] args) {
        if (args.length != 3) {
            System.err.println("Usage: java -jar creeps.jar <hostname> <port> <login>");
            System.exit(1);
        }
        
        String hostname = args[0];
        int port = Integer.parseInt(args[1]);
        String login = args[2];
        
        System.out.println("Connexion à " + hostname + ":" + port + " avec le login: " + login);
        
    }
}
