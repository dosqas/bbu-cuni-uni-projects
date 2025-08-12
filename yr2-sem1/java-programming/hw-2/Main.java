package cz.cuni.mff.sopteles.programs;

import cz.cuni.mff.sopteles.util.DynamicArray;

public class Main {
    public static void main(String[] args) {
        DynamicArray DA = new DynamicArray();
        int ind = 0;
        while (ind < args.length) {
            DA.add(args[ind]);
            ind++;
        }

        ind = 0;
        while (DA.get(ind) != null) {
            System.out.println(DA.get(ind));
            ind++;
        }
    }
}