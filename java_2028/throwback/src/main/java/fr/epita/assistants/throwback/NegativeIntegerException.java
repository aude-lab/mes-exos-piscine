package fr.epita.assistants.throwback;

public class NegativeIntegerException extends IntegerException {
    public NegativeIntegerException(String message) {
        super("NegativeIntegerException: " + message);
    }

}
