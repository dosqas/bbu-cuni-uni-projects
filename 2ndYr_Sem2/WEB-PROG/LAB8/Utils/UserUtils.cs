using System.Security.Claims;

namespace LAB8.Utils;

public static class UserUtils
{
    public static string GetUserId(ClaimsPrincipal user)
        => user.FindFirstValue(ClaimTypes.NameIdentifier)!;
}