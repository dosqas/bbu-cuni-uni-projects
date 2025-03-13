package cz.cuni.mff.sopteles.programs;

import cz.cuni.mff.sopteles.util.MString;

public class Main {
    public static void main(String[] args) {
        MString mStr = new MString();

        for (int ind = 0; ind < args.length; ind++) {
            try {
                switch (args[ind]) {
                    case "a": {
                        mStr.append(args[++ind]);
                        break;
                    }
                    case "i": {
                        int pos = Integer.parseInt(args[++ind]);
                        mStr.insert(pos, args[++ind]);
                        break;
                    }
                    case "d": {
                        int pos = Integer.parseInt(args[++ind]);
                        int length = Integer.parseInt(args[++ind]);
                        mStr.delete(pos, length);
                        break;
                    }
                }
            }
            catch (Exception e) {
                System.out.println("INPUT ERROR");
                System.exit(0);
            }
        }

        System.out.println(mStr);
    }
}