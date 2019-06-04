SELECT Employee.FirstName, Employee.LastName, SuperEmployee.FirstName AS SuperFirst, SuperEmployee.LastName AS SuperLast, SuperEmployee.Title AS SuperTitle
FROM Employee
LEFT JOIN Employee AS SuperEmployee ON Employee.ReportsTo = SuperEmployee.EmployeeId