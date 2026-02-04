package fr.epita.assistants.throwback;

public class Pitcher {
    public static void throwException(String message) throws LongStringException, ShortStringException, PositiveIntegerException, NegativeIntegerException, UnknownException {
        if (message.matches("-?\\d+")) {
            if (message.charAt(0) == '-') {
                throw new NegativeIntegerException(message);
            } else {
                throw new PositiveIntegerException(message);
            }
        }

        if (message.matches("[a-zA-Z\\s,.']*")) {
            if (message.length() >= 100) {
                throw new LongStringException(message);
            }
            else {
                throw new ShortStringException(message);
            }
        }
        throw new UnknownException(message);
    }


}
