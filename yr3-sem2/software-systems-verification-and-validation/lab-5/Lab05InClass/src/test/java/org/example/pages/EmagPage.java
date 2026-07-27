package org.example.pages;

import net.thucydides.core.annotations.DefaultUrl;
import net.serenitybdd.core.pages.WebElementFacade;
import net.serenitybdd.core.annotations.findby.FindBy;
import net.thucydides.core.pages.PageObject;
import org.openqa.selenium.By;
import org.openqa.selenium.support.ui.ExpectedConditions;

@DefaultUrl("https://www.emag.ro/")
public class EmagPage extends PageObject {

    @FindBy(id="searchboxTrigger")
    WebElementFacade searchInput;

    @FindBy(css=".title-phrasing")
    WebElementFacade searchResultMessage;

    public void searchFor(String keyword) {
        searchInput.waitUntilClickable().clear();
        searchInput.typeAndEnter(keyword);
    }

    public void applyFilter(String filterName) {
        String filterXPath = "//a[@data-name='" + filterName + "']";
        WebElementFacade filterElement = find(By.xpath(filterXPath));

        evaluateJavascript("arguments[0].scrollIntoView({block: 'center'});", filterElement);

        try {
            filterElement.waitUntilClickable().click();
        } catch (Exception e) {
            evaluateJavascript("arguments[0].click();", filterElement);
        }
    }

    public String getSearchResultMessage(String expectedText) {
        waitFor(ExpectedConditions.textToBePresentInElement(searchResultMessage, expectedText));

        return searchResultMessage.getText();
    }
}