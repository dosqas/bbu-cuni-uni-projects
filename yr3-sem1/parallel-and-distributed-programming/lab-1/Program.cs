namespace lab_1;

public class Account
{
    public int Balance { get; set; }
    public readonly object Lock = new();
}

public class Program
{
    readonly static int totalAccounts = 20;
    readonly static int totalTransactions = 10_000_000;
    static Account[]? accounts;
    static bool running = true;
    static readonly object globalLock = new();

    public static void Main()
    {
        int initialBalance = 1000;
        accounts = new Account[totalAccounts];
        for (int i = 0; i < accounts.Length; i++)
            accounts[i] = new Account { Balance = initialBalance };

        var checkTask = Task.Run(() =>
        {
            while (running)
            {
                ConsistencyCheck(initialBalance);
                Thread.Sleep(1);
            }
        });

        var transferTasks = new List<Task>();

        for (int i = 0; i < totalTransactions; i++)
        {
            int from = i % totalAccounts;
            int to = (i * 7) % totalAccounts;
            Random random = new();
            int amount = random.Next(1, 100);

            if (from == to) to = (to + 1) % totalAccounts;

            var t = Task.Run(() =>
            {
                var first = Math.Min(from, to);
                var second = Math.Max(from, to);

                lock (accounts[first].Lock)
                {
                    lock (accounts[second].Lock)
                    {
                        if (accounts[from].Balance >= amount)
                        {
                            Transfer(from, to, amount);
                        }
                    }
                }
            });
            transferTasks.Add(t);
        }

        Task.WaitAll([.. transferTasks]);

        running = false;
        checkTask.Wait();
        ConsistencyCheck(initialBalance);
    }

    static void Transfer(int from, int to, int amount)
    {
        lock (globalLock)
        {
            if (accounts![from].Balance >= amount)
            {
                accounts[from].Balance -= amount;
                accounts[to].Balance += amount;
            }
        }
    }

    static void ConsistencyCheck(int initialBalance)
    {
        lock (globalLock)
        {
            int total = 0;
            foreach (var acc in accounts!)
                total += acc.Balance;
            Console.WriteLine($"[Check] Total balance: {total} (should be {totalAccounts * initialBalance})");
        }
    }
}