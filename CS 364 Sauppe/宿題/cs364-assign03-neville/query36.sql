SELECT BillingCountry AS Country, Title, count(AlbumId) AS PurchaseCount
FROM Invoice NATURAL JOIN InvoiceLine NATURAL JOIN Track NATURAL JOIN Album 
GROUP BY Country, AlbumId
ORDER BY PurchaseCount DESC;
