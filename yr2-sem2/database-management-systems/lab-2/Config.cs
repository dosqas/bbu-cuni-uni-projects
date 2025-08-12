namespace LAB2
{
    internal class Config
    {
        public string? ConfigName { get; set; }
        public string? FormCaption { get; set; }
        public string? MasterQuery { get; set; }
        public string? DetailQuery { get; set; }
        public string? MasterTableName { get; set; }
        public string? DetailTableName { get; set; }
        public RelationConfig? Relation { get; set; }
    }

    internal class RelationConfig
    {
        public string? MasterColumnName { get; set; }
        public string? DetailColumnName { get; set; }
    }
}
