# Restaurant management software

A project made for completing the course Introduction to programming - practicum at FMI, Sofia University.

## Overview

This is a simple restaurant management system implemented in C++. The system allows restaurant staff to manage orders, stock, and menu items efficiently. The data is stored in text files.

## User roles

 **Waiter**, who is responsible for handling customer orders. The functionalities available to a waiter include:
 
- Viewing the menu
- Placing orders for customers
- Canceling an order
- Viewing sorted orders
- Viewing the daily turnover

**Manager**, who has additional privileges beyond those of a waiter. The manager can:

- Add new items to the menu
- Remove items from the menu
- Take inventory reports
- Update stock quantities

## File Structure

The system stores data in the following files:

*menu.txt*: Stores menu items in the format DishName,Price:Ingredient1,Quantity;Ingredient2,Quantity;...

*orders.txt*: Logs placed orders with name of the dish and date.

*stock.txt*: Stores available ingredient stock in the format Ingredient,Quantity.

*date.txt*: Tracks the last recorded date for daily turnover calculations.
