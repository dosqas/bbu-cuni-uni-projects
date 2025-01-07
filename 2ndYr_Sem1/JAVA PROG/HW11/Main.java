package cz.cuni.mff.sopteles.programs;

import cz.cuni.mff.sopteles.util.BFInterpreter;

public class Main {
    public static void main(String[] args) {
        int memory = 30000;
        if (args.length != 1) {
            memory = Integer.parseInt(args[1]);
        }

        BFInterpreter.setMemory(memory);

        String fileName = args[0];

        String returnMessage = BFInterpreter.validateProgram(fileName);
        if (!returnMessage.isEmpty()) {
            System.out.print(returnMessage);
            return;
        }

        BFInterpreter.runProgram(fileName);
    }
}