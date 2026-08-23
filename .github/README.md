# CalQ

Track how many calories you eat, your macronutrients and how much you exercise.
This program allows you to log your meals and weight, create recipes for ease of use and track how much of a meal you have eaten, if you did not finish it all.
As you use CalQ, you will build up a database of different foods and exercises, which enables you to quickly add them again.
You can display graphs showing you food consumption, weight and how much was exercised on each day.
![Image of the main tab](/.github/Main.png)

As you use the program you gradually build your own database of foods, recipes and exercises. Additionally there are two ways to import foods into your local database:
- Connect to an SQL server and either import all the data or search for single foods. The program can be configured to accept different table and field names to make this simple.
- Ask AI which is locally hosted using LMStudio/Bionic. You will receive a (hopefully accurate) result and can decide to import it into the local database. An AI model which I found to work well for this is Gemma 4 E4B, which tends to be both accurate and quick and needs less than 8GB of memory.

![Image of the database tab](/.github/Database.png)

Built using C++ and Qt 6.11.