using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Authorization;

namespace LAB8.Controllers.Mvc;

[Authorize]
public class ViewAdminController : Controller
{
    public IActionResult Index()
    {
        return View();
    }
}
