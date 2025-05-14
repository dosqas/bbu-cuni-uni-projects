using LAB8.Data;
using LAB8.Models;
using LAB8.Services.Interfaces;
using Microsoft.EntityFrameworkCore;

namespace LAB8.Services;

public class AuthService : IAuthService
{
    private readonly AppDbContext _context;

    public AuthService(AppDbContext context)
    {
        _context = context;
    }

    public async Task<User?> AuthenticateAsync(string username, string password)
    {
        return await _context.Users
            .FirstOrDefaultAsync(u => u.Username == username && u.Password == password);
    }

    public async Task<User> RegisterAsync(string username, string password)
    {
        var user = new User { Username = username, Password = password };
        _context.Users.Add(user);
        await _context.SaveChangesAsync();
        return user;
    }
}