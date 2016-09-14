using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Rafiki_Self_Help
{
    class program
    {
        //Variables used for data storage in an external file
        //-------------------------------------------------------------------//
        public static string DataFile = "storage.txt";
        public static string Content = "(Empty File)";
        public static string Input = "";
        public static bool Run = true;
        public static int Choice = 0;

        static void ColorText(string Message)
        {
            Console.ForegroundColor = ConsoleColor.Blue;
            Console.BackgroundColor = ConsoleColor.White;
            Console.WriteLine(Message);
            Console.ResetColor();
        }
        //--------------------------------------------------------------------//


        //note: separate the different modules into methods
        static void Main(string[] args)
        {
            //Accessing The records.

            //This will display current record and register new members
            //-----------------------------------------------------------------------//
            Console.Title = "Rafiki Self Help System";

            while (Run == true)
            {
                Records();
            }
            //---------------------------------------------------------------------//
        }

        //Records() method accessing the file
        //---------------------------------------------------------------------------------------------//
        static void Records()
        {
            double loan = 0;
            string Input = "";
            int Choice = 0;

            Console.WriteLine("\n\n\n\n\n------------------------------------------");
            Console.WriteLine(" 1) Read Member Records\n2) Register new member\n3) Search for an existing record\n4) Loan Facility\n");
            Console.WriteLine("-------------------------------------------------------------------------------------------");
            Input = Console.ReadLine();
            Choice = Convert.ToInt32(Input);

            //Viewing registered Members
            //----------------------------------------------------------------------------------------//
            if (Choice == 1)
            {
                Console.Clear();
                if (File.Exists(DataFile))
                {
                    ColorText("\n\nThe file contents are:");
                    Content = File.ReadAllText(DataFile);
                }
                Console.WriteLine(Content, true);
            }
            //--------------------------------------------------------------------------------------//

            //Registration of a new member
            //---------------------------------------------------------------------------------//
            else if (Choice == 2)
            {
                Console.Clear();
                ColorText("\n\nEnter new members Name\tAmount in Acc\tTime as member");
                Input = Console.ReadLine();
                using (StreamWriter sw = File.AppendText(DataFile))
                {
                    Content = Input;
                    ColorText("File updated.");
                    sw.WriteLine(Content);
                    Console.WriteLine(Content + " Has been registered as a member");
                }
            }
            //-----------------------------------------------------------------------------------//

            //Searches through the records
            //-------------------------------------------------------------------------------------//
            else if (Choice == 3)
            {
                int counter = 0;
                string line;

                // Read the line by line.
                System.IO.StreamReader file =
                    new System.IO.StreamReader(DataFile);

                Console.WriteLine("Enter members name");
                string member_name = Console.ReadLine();

                while (!file.EndOfStream)
                {
                    line = file.ReadLine();
                    //split string on tab (\t)
                    string[] words = line.Split(';');
                    if (words.Contains(member_name))
                    {
                        int amount = int.Parse(words[1]);
                        Console.WriteLine(amount);
                    }
                    else
                    {

                    }
                }
                counter++;

                file.Close();
                // Suspend the screen.
                Console.ReadLine();
            }
            //Loan Facility
            //--------------------------------------------------------------------------------------//
            else if (Choice == 4)
            {
                int counter = 0;
                string line;
                //Get the user's name
                Console.WriteLine("Enter members name");
                string member_name = Console.ReadLine();
                // Read the line by line.
                System.IO.StreamReader file =
                    new System.IO.StreamReader(DataFile);
                while (!file.EndOfStream)
                {
                    line = file.ReadLine();
                    //split string on tab (\t)
                    string[] words = line.Split(';');
                    if (words.Contains(member_name))
                    {
                        //First Condition for the loop is met i.e. Client is a Member
                        //------------------------------------------------------------------------------//
                        int time = Convert.ToInt32(words[1]);
                        //Console.WriteLine(time);
                        //Converts the time string to integer
                        if (time > 3)
                        {
                            Console.WriteLine("Member can apply for a loan");
                            Console.WriteLine("What type of loan would you like to apply for? (1,2 or 3)");
                            Console.WriteLine("|-----------------------------------------------------------|");
                            Console.WriteLine("| No.  | Type       | Rate  | Max Amount| Max Payment Period|");
                            Console.WriteLine("|------|------------|-------|-----------|-------------------|");
                            Console.WriteLine("| 1.   | Investment | 12%   | 100,000   | 3 years/ 36 months|");
                            Console.WriteLine("| 2.   | Emergency  | 8%    | 50,000    | 1 year/ 12 months |");
                            Console.WriteLine("| 3.   | Education  | 10%   | 60,000    | 2 years/ 24 months|");
                            Console.WriteLine("|------|------------|-------|-----------|-------------------|");
                            int ans = int.Parse(Console.ReadLine());
                            if (ans == 1)
                            {
                                Console.WriteLine("<------Investment Loan------->");
                                Console.WriteLine("How much would you like to borrow");
                                loan = Convert.ToDouble(Console.ReadLine());
                                //Check for the amount in the account
                                //---------------------------------------------------------------------------//
                                int amount = Convert.ToInt32(words[1]); //convert the amount column to a variable of type integer.
                                float repayment;
                                if ((loan <= 100000.0) || (loan <= 3.0 * amount))
                                {
                                    repayment = (float)((loan * 0.12 * 3.0) + loan);

                                    Console.WriteLine("Applicant's Name:\t" + member_name);
                                    Console.WriteLine("Type of Loan applied for:\tInvestment");
                                    Console.WriteLine("Amount applied for:\t" + loan);
                                    Console.WriteLine("Applicant's Name:\t3 years / 36 months");
                                    Console.WriteLine("Total amount to be repaid:\t" + repayment);
                                    Console.WriteLine("Loan Status:\tApproved");
                                    break;
                                } else { Console.WriteLine("Amount requested to high you need to cut it"); }
                            }
                            else if (ans == 2)
                            {
                                Console.WriteLine("<------Emergency Loan------->");
                                Console.WriteLine("How much would you like to borrow");
                                loan = Convert.ToDouble(Console.ReadLine());
                                //Check for the amount in the account
                                //---------------------------------------------------------------------------//
                                int amount = Convert.ToInt32(words[1]); //convert the amount column to a variable of type integer.
                                float repayment;
                                if (loan <= 50000.0)
                                {
                                    repayment = (float)((loan * 0.08 * 1.0) + loan);

                                    Console.WriteLine("Applicant's Name:\t" + member_name);
                                    Console.WriteLine("Type of Loan applied for:\tEmergency");
                                    Console.WriteLine("Amount applied for:\t" + loan);
                                    Console.WriteLine("Applicant's Name:\t1 year / 12 months");
                                    Console.WriteLine("Total amount to be repaid:\t" + repayment);
                                    Console.WriteLine("Loan Status:\tApproved");
                                    break;
                                } else { Console.WriteLine("Amount requested to high you need to cut it"); }
                            }
                            else if (ans == 3)
                            {
                                Console.WriteLine("<------Education Loan------->");
                                Console.WriteLine("How much would you like to borrow");
                                loan = Convert.ToDouble(Console.ReadLine());
                                //Check for the amount in the account
                                //---------------------------------------------------------------------------//
                                int amount = Convert.ToInt32(words[1]); //convert the amount column to a variable of type integer.
                                float repayment;
                                if ((loan <= 60000.0) || (loan <= 3.0 * amount))
                                {
                                    repayment = (float)((loan * 0.1 * 2.0) + loan);

                                    Console.WriteLine("Applicant's Name:\t" + member_name);
                                    Console.WriteLine("Type of Loan applied for: Education");
                                    Console.WriteLine("Amount applied for:\t" + loan);
                                    Console.WriteLine("Applicant's Name:\t2 years / 24 months");
                                    Console.WriteLine("Total amount to be repaid:\t" + repayment);
                                    Console.WriteLine("Loan Status:\tApproved");
                                    break;
                                } else {Console.WriteLine("Amount requested to high you need to cut it"); }
                            }
                            else { Console.WriteLine("Invalid Entry"); }
                        }
                        else
                        {
                            Console.WriteLine("That time is way too short you need to add it");
                            break;// time condition not met
                        }
                    } else if(!words.Contains(member_name)){
                        continue;
                    }
                    }
                    counter++;
                }
            //-------------------------------------------------------------------------------------//
            else
            {
                Run = false;
            }
        }
        //----------------------------------------------------------------------------------------------//
    }
}