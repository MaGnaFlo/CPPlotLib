import sqlite3
import numpy as np
import matplotlib.pyplot as plt
from collections import defaultdict

def plot_macros(type):
    con = sqlite3.connect("macro.db")
    cur = con.cursor()

    entries = cur.execute("SELECT * from Entries")
    entries_data = entries.fetchall()
    entries_cols = list(map(lambda x: x[0], cur.description))
    entries = defaultdict(list)
    for data in entries_data[::-1]:
        entry = {col: val for col, val in list(zip(entries_cols, data)) if col != 'ID' and col != 'Date'}
        entries[data[1]].append(entry)

    dates = list(entries.keys())
    entries_macros = []
    for _, entry in entries.items():
        macros = {"Saturated_fats": 0,
                "Unsaturated_fats": 0,
                "Carbohydrates": 0,
                "Proteins": 0}
        for e in entry:
            for key in macros.keys():
                macros[key] += e[key]
        entries_macros.append(macros)

    colors = {"Saturated_fats": 'darkgoldenrod',
            "Unsaturated_fats": 'goldenrod',
            "Carbohydrates": 'forestgreen',
            "Proteins": 'firebrick'}
    calories = {"Saturated_fats": 9.02,
            "Unsaturated_fats": 9.04,
            "Carbohydrates": 3.5,
            "Proteins": 4.0}

    _, ax = plt.subplots(figsize=(15,7))
    for date, macro in list(zip(dates, entries_macros)):
        offset = 0
        for name, g in macro.items():
            if type == 'calories':
                value = g*calories[name]
            else:
                value = g
            ax.bar(date, value, bottom=offset, color=colors[name])
            offset += value

    plt.legend(list(colors.keys()))
    plt.xticks(rotation=90)
    if type == 'calories':
        plt.ylabel("Energy (calories)")
    else:
        plt.ylabel("Mass (g)")
    plt.tight_layout()
    plt.show()

if __name__ == '__main__':
    plot_macros(type='calories')