using c_a1a2;

namespace cs_a1a2
{
    internal class Program
    {
        public static void Main(string[] args)
        {
            InputValidator valid = new InputValidator();
            ErrHandler errHandle = new ErrHandler();

            bool validity = valid.ValidateCommandLineArguments(args);
            if (!validity)
            {
                errHandle.HandleError("cl");
            }

            ReadFromCL readCl = new ReadFromCL();
            string filename = readCl.ReadArgs(args);

            validity = valid.ValidateFile(filename);
            if (!validity)
            {
                errHandle.HandleError("file");
            }

            ReadFromFile read = new ReadFromFile();
            CalculateWCPara wc = new CalculateWCPara();
            FormCharsIntoStrings form = new FormCharsIntoStrings();
            WriteToSTDOUT writer = new WriteToSTDOUT();

            read.ReadFile(filename, processReadChar =>
            {
                string processResult = form.ProcessChar(processReadChar);
                if (processResult == "WORD_ENDED")
                {
                    wc.AddCount();
                }
                else if (processResult == "NEW_PARA")
                {
                    if (wc.getCurrentWC() != 0)
                    {
                        writer.WriteOutput(wc.getCurrentWC());
                        wc.Calculate();
                    }
                }
            });

            if (form.ProcessChar('\0') == "WORD_ENDED")
            {
                wc.AddCount();
                writer.WriteOutput(wc.getCurrentWC());
            }
            else if (wc.getCurrentWC() != 0)
            {
                writer.WriteOutput(wc.getCurrentWC());
                wc.Calculate();
            }
        }
    }
}
