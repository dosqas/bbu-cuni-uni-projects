using Microsoft.AspNetCore.Mvc;
using System.Security.Claims;
using Microsoft.AspNetCore.Authentication;
using Microsoft.AspNetCore.Authentication.Cookies;
using LAB8.Services.Interfaces;

namespace LAB8.Controllers;

public class AuthController(IAuthService authService) : Controller
{
    private readonly IAuthService _authService = authService;

    // Login GET
    [HttpGet]
    public IActionResult Login()
    {
        return View();
    }

    // Login POST
    [HttpPost]
    public async Task<IActionResult> Login(string username, string password)
    {
        var user = await _authService.AuthenticateAsync(username, password);
        if (user != null)
        {
            var claims = new List<Claim>
            {
                new(ClaimTypes.Name, user.Username),
                new(ClaimTypes.NameIdentifier, user.Id.ToString())
            };

            var claimsIdentity = new ClaimsIdentity(claims, CookieAuthenticationDefaults.AuthenticationScheme);

            await HttpContext.SignInAsync(
                CookieAuthenticationDefaults.AuthenticationScheme,
                new ClaimsPrincipal(claimsIdentity));

            return RedirectToAction("Index", "Browse");
        }

        ViewBag.Error = "Invalid username or password.";
        return View();
    }

    // Register GET
    [HttpGet]
    public IActionResult Register()
    {
        return View();
    }

    // Register POST
    [HttpPost]
    public async Task<IActionResult> Register(string username, string password)
    {
        // Optionally: check if user exists first
        var user = await _authService.RegisterAsync(username, password);
        if (user != null)
        {
            // Auto-login after registration
            var claims = new List<Claim>
            {
                new(ClaimTypes.Name, user.Username),
                new(ClaimTypes.NameIdentifier, user.Id.ToString())
            };

            var claimsIdentity = new ClaimsIdentity(claims, CookieAuthenticationDefaults.AuthenticationScheme);

            await HttpContext.SignInAsync(
                CookieAuthenticationDefaults.AuthenticationScheme,
                new ClaimsPrincipal(claimsIdentity));

            return RedirectToAction("Index", "Cart");
        }

        ViewBag.Error = "Registration failed.";
        return View();
    }

    [HttpPost]
    public async Task<IActionResult> Logout()
    {
        await HttpContext.SignOutAsync(CookieAuthenticationDefaults.AuthenticationScheme);
        return RedirectToAction("Login");
    }
}