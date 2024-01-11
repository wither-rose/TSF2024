# A short script to allow the user to determine what % sterilized their object is
# given the amount of time their object has been submerged in boiling water
# based on the regression line from our experiment

# store the regression line equation from data processing
m = -0.8075867411379077 # regression coefficient
b = 357.3532013337096 # regression constant

# information
print("\t\tSterilization Percentage Calculator")
print("--------------------------------------------")
print("This program can calculate the percentage that your baby bottle is sterilized based on how long you submerged the bottle in boiling water.")

while True:
    # take input from the user for the independent variable
    x = input("Please enter the length of time you submerged your bottle in boiling water: ")
    # error trap in case the user does not enter a number value
    try:
        x = float(x)
    except ValueError:
        print("Please enter a numeric value!")
        continue
    # error trap in case the user does not enter a positive number
    if x < 0:
        print("Please enter a positive number!")
        continue

    print("Calculating...")
    # calculate the regression line equation
    y = m*x+b
    # calculate the percentage as a percentage of the regression constant subtracted from 100
    percentage = 100-int(y/b*100)
    # output the results
    if percentage > 100:
        percentage = 100
    print("Your baby bottle is", str(percentage)+"%", "sterilized!")
    # see if the user wants to continue
    cont = ""
    while cont != "Y" and cont != "N":
        cont = input("Do you want to calculate again? (Y/N) ")
    # end the loop if the user doesn't want to continue
    if cont == "N":
        break
print("Thank you for using this program!")

