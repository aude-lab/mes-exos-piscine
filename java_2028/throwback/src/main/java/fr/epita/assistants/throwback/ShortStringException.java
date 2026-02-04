package fr.epita.assistants.throwback;

public class ShortStringException extends StringException {
    public ShortStringException(String message) {
        super("ShortStringException: " + message + " (length: " + message.length() + ")");
    }
}
