#!/usr/bin/python3.8 

import tkinter as tk
from tkinter import scrolledtext, ttk, filedialog, messagebox
import numpy as np
from numpy import loadtxt
import matplotlib.pyplot as plt
import networkx as nx
import os
from os import popen, remove, path
from PIL import ImageTk, Image

entries_compartments = []
entries_parameters = []
entries_ode       = []
entries_values_parameters = []
initial_condition = []
name_parameter    = []
name_compartment  = []
name_compartment2 = []
network_parameters= []
label             = []
value_dv          = []
config_values     = []
label1            = []
label2            = []
label21           = []
filename          = []
colors            = ["black","blue","red","green","brown","orange","aqua","magenta","lawngreen","purple"]
compartment_models= [["S","I"],["S","I"],["S","I","R"],["S","I","R"],["S","E","I","R"],["S","E","I","R"]]
compartment_parameters = [["beta"],["beta", "gamma"],["beta", "gamma"],["beta", "gamma", "rho"],["beta", "gamma", "rho"],["beta", "gamma", "rho", "epsilon"]]
aux_info = 0

class ToolTip(object):

    def __init__(self, widget):
        self.widget = widget
        self.tip_window = None
        self.id = None
        self.text = None
        self.x = self.y = 0

    def showtip(self, text, x_dist, y_dist):
        self.text = text
        if self.tip_window or not self.text:
            return
        x, y, cx, cy = self.widget.bbox('insert')
        x = x + self.widget.winfo_rootx() + x_dist
        y = y + cy + self.widget.winfo_rooty() + y_dist
        self.tip_window = tw = tk.Toplevel(self.widget)
        tw.wm_overrideredirect(1)
        tw.wm_geometry('+%d+%d' % (x, y))
        label = tk.Label(tw, text=self.text, justify=tk.LEFT, background='#ffffe0', relief=tk.SOLID,
                         borderwidth=1)
        label.pack(ipadx=1)

    def hidetip(self):
        tw = self.tip_window
        self.tip_window = None
        if tw:
            tw.destroy()

def create_tool_tip(widget, text, x_dist=20, y_dist=15):
    tool_tip = ToolTip(widget)

    def enter(event):
        tool_tip.showtip(text, x_dist, y_dist)

    def leave(event):
        tool_tip.hidetip()

    widget.bind('<Enter>', enter)
    widget.bind('<Leave>', leave)

def compartments_names(window,model):

    for i in entries_compartments:
        i.delete(0, tk.END)
        i.place_forget()
    
    for i in initial_condition:
        i.delete(0, tk.END)
        i.place_forget()

    for i in entries_ode:
        i.delete(0, tk.END)
        i.place_forget()

    for i in name_compartment:
        i.place_forget()

    for i in name_compartment2:
        i.place_forget()

    entries_compartments.clear()
    initial_condition.clear()
    entries_ode.clear()
    name_compartment.clear()
    name_compartment2.clear()

    number_compartments = int(model.get())
    for j in range(0, int(number_compartments/5)+1) :
        for i in range(0, 5) :
            my_entry = tk.Entry(window,width=3)
            my_entry.place(x = 200+70*i, y = 70+30*j)
            entries_compartments.append(my_entry)
            number_compartments=number_compartments-1
            if number_compartments == 0:
                break
        if number_compartments == 0:
            break

def parameters_names(window,model):
    for i in entries_parameters:
        i.delete(0, tk.END)
        i.place_forget()

    for i in entries_values_parameters:
        i.delete(0, tk.END)
        i.place_forget()
    
    for i in name_parameter:
        i.place_forget()

    entries_parameters.clear()
    entries_values_parameters.clear()
    name_parameter.clear()

    number_parameters = int(model.get())
    for j in range(0, int(number_parameters/5)+1) :
        for i in range(0, 5) :
            my_entry = tk.Entry(window,width=7)
            my_entry.place(x = 200+70*i, y = 160+30*j)
            entries_parameters.append(my_entry)
            number_parameters=number_parameters-1
            if number_parameters == 0:
                break
        if number_parameters == 0:
            break

def dinamics_vital_function(window,model):
    
    for i in value_dv:
        i.delete(0, tk.END)
        i.place_forget()

    for i in label1:
        i.place_forget()

    value_dv.clear()

    if model.get() == "within":
        label1.append(tk.Label(window, text=u'\u03BC: '))
        label1[0].place(x = 750, y = 160)
        value_dv.append(tk.Entry(window,width=7))
        value_dv[0].place(x = 810, y = 160)

def numberOnlyInt(valores):
    for entries in valores:
        if not(entries.get().isdigit()):
            messagebox.showerror("Wrong data","Invalid initial condition, only integers")
            return False
        else:
            return True

def numberOnlyFloat(valores):
    for entries in valores:
        if not(entries.get().replace(".", "", 1).isdigit()):
            messagebox.showerror("Wrong data","Invalid parameter value, real numbers only")    
            return False
        else:
            return True

def numberOfDinamicVital(valor):
    if not(valor.replace(".", "", 1).isdigit()):
        messagebox.showerror("Wrong data","Invalid value of vital dynamic, real numbers only")    
        return False
    else:
        return True

def ode_system(window, code):

    if code == 1000:
        j=0
        for i in range(0,len(entries_compartments)):
            name1 = tk.Label(window, text=  str(entries_compartments[i].get()) + u"\u030a" + ": ")
            name1.place(x = 8, y = 240+30*j)
            my_entry = tk.Entry(window,width=50)
            my_entry.place(x = 28, y = 240+30*j)
            entries_ode.append(my_entry)
            name_compartment.append(name1)

            name2 = tk.Label(window, text=str(entries_compartments[i].get()) + "(0): ")
            name2.place(x = 460, y = 240+30*j)
            my_entry2 = tk.Entry(window,width=6)
            my_entry2.place(x = 500, y = 240+30*j)
            initial_condition.append(my_entry2)
            name_compartment2.append(name2)
            j=j+1
    else:
        j=0
        for i in range(0,len(compartment_models[code])):
            name2 = tk.Label(window, text=str(compartment_models[code][i]) + "(0): ")
            name2.place(x = 460, y = 240+30*j)
            my_entry2 = tk.Entry(window,width=6)
            my_entry2.place(x = 500, y = 240+30*j)
            initial_condition.append(my_entry2)
            name_compartment2.append(name2)
            j=j+1

def value_parameters(window, code):

    if code == 1000:
        j=0
        l=0
        for i in range(0,len(entries_parameters)):
            name1 = tk.Label(window, text=str(entries_parameters[i].get()) + ": ")
            name1.place(x = 750+120*l, y = 10+(j+1)*30)
            my_entry = tk.Entry(window,width=7)
            my_entry.place(x = 810+120*l, y = 10+(j+1)*30)
            entries_values_parameters.append(my_entry)
            name_parameter.append(name1)
            j=j+1
            if j==2:
                j=0
                l=l+1
    else:
        j=0
        l=0
        for i in range(0,len(compartment_parameters[code])):
            name1 = tk.Label(window, text=str(compartment_parameters[code][i]) + ": ")
            name1.place(x = 750+120*l, y = 10+(j+1)*30)
            my_entry = tk.Entry(window,width=7)
            my_entry.place(x = 810+120*l, y = 10+(j+1)*30)
            entries_values_parameters.append(my_entry)
            name_parameter.append(name1)
            j=j+1
            if j==2:
                j=0
                l=l+1

def configuration_simulation(window):
    tk.Label(window, text="Number of simulation: ").place(x = 750, y = 220)
    config_values.append(tk.Entry(window,width=7))
    config_values[0].place(x = 930, y = 220)
    
    tk.Label(window, text="Time spacing: ").place(x = 750, y = 250)
    config_values.append(tk.Entry(window,width=7))
    config_values[1].place(x = 930, y = 250)
    
    tk.Label(window, text="Final time: ").place(x = 750, y = 280)
    config_values.append(tk.Entry(window,width=7))
    config_values[2].place(x = 930, y = 280)

def size_validate():
    global size_arq
    ans = 0
    for i in range(0,len(initial_condition)):
        ans = ans + int(initial_condition[i].get())

    if size_arq == ans:
        return True
    else:
        messagebox.showerror("Wrong data","Different population sizes")  
        return False

def run_simulation(btn2,btn3,dinamics_vital,network,answer,model):
    a1 = numberOnlyInt(initial_condition)
    a2 = numberOnlyFloat(entries_values_parameters)
    a3 = True
    if network.get() == "other":
        a3 = size_validate()
    a4 = True
    if dinamics_vital.get() == "within":
        a4 = numberOfDinamicVital(value_dv[0].get())

    if (a1 and a2 and a3 and a4):
        my_file = open("IBM.dat", "w")
        my_file.write(config_values[1].get())
        my_file.write("\n")

        if answer== "no":
            my_file.write(str(len(entries_compartments)))
            my_file.write("\n")
            for entries in entries_compartments:
                my_file.write(str(entries.get())+" ")
            my_file.write("\n")
            my_file.write(str(len(entries_parameters)))
            my_file.write("\n")
            for entries in entries_parameters:
                my_file.write(str(entries.get())+" ")
            my_file.write("\n")
            for entries in entries_ode:
                my_file.write(str(entries.get())+" ;\n")
        else :
            f = open("models/IBM_" + str(model.current()) + ".dat", "r")
            my_file.write(f.read())

        for entries in entries_values_parameters:
            my_file.write(str(entries.get())+"\n")
        for entries in initial_condition:
            my_file.write(str(entries.get())+"\n")
        my_file.close()

        ans = 0
        for i in range(0,len(initial_condition)):
            ans = ans + int(initial_condition[i].get())

        net_par = ''
        for i in range(0,len(network_parameters)):
            net_par = net_par + ' ' + str(network_parameters[i].get())

        if network.get() == "Random":
            popen('models\\ran.exe ' + str(ans) + net_par).read()
        elif network.get() == "Small-World":
            popen('models\\smw.exe ' + str(ans) + net_par).read()
        elif network.get() == "Scale-Free":
            popen('models\\scf.exe ' + str(ans) + net_par).read()
        elif network.get() == "Modular":
            popen('models\\mod.exe ' + str(ans) + net_par).read()
        elif network.get() == "Hierarchical":
            popen('models\\hie.exe ' + str(ans) + net_par).read()
        else:
            popen('models\\other.exe ' + '"' + filename[0] + '"').read()

        output = popen('IBM_WRITER.exe').read()

        a = ''
        for i in range(0,len(config_values)):
            a = a + ' ' + str(config_values[i].get())

        if dinamics_vital.get() == "without":
            output = popen('IBM_MAIN.exe ' + a + ' ' + str(ans) + ' N').read()
        else:
            output = popen('IBM_MAIN.exe ' + a + ' ' + str(ans) + ' Y '+ str(value_dv[0].get())).read()

        btn2['state'] = tk.NORMAL
        btn3['state'] = tk.NORMAL
        plot_results(answer,model)
        

def writer_system(window):

    entries_ode.clear()
    initial_condition.clear()
    entries_values_parameters.clear()

    ode_system(window,1000)
    value_parameters(window,1000)

def network_chosen(window,model,btn1,information):
    network = model.get()
    global aux_info

    for i in network_parameters:
        i.delete(0, tk.END)
        i.place_forget()
    
    for i in label:
        i.place_forget()

    network_parameters.clear()
    label.clear()
    information['state'] = tk.NORMAL

    information.delete("end-"+str(aux_info)+"c", tk.END)
    text_before = len(information.get("1.0", tk.END))
    
    if network == "Random":
        label.append(tk.Label(window, text="p: "))
        label[0].place(x = 750, y = 370)
        create_tool_tip(label[0], text='Probability of connection')
        network_parameters.append(tk.Entry(window,width=5))
        network_parameters[0].place(x = 800, y = 370)
        network_parameters[0].insert(tk.END, '1')
        information.insert(tk.INSERT, '\nNETWORK : ERDOS, P.; RENYI A. On random graphs I. Publicationes Mathematicae (Debrecen), v. 6, p. 290-297, 1959.\n')

    elif network == "Small-World":
        label.append(tk.Label(window, text="p: "))
        label[0].place(x = 750, y = 370)
        create_tool_tip(label[0], text='Probability of rewiring each edge')
        label.append(tk.Label(window, text='n'+u'\u2080: '))
        label[1].place(x = 950, y = 370)
        create_tool_tip(label[1], text='Each node is connected to n'+u'\u2080: '+' nearest\nneighbors in ring topology')
        network_parameters.append(tk.Entry(window,width=5))
        network_parameters[0].place(x = 800, y = 370)
        network_parameters[0].insert(tk.END, '1')
        network_parameters.append(tk.Entry(window,width=5))
        network_parameters[1].place(x = 1000, y = 370)
        network_parameters[1].insert(tk.END, '1')
        information.insert(tk.INSERT, '\nNETWORK : WATTS, D. J.; STROGATZ, S. H. Collective dynamics of ''small-world'' networks. Nature, v. 393, n. 6684, p. 440, 1998.\n')

    elif network == "Scale-Free":
        label.append(tk.Label(window, text='n'+u'\u2080: '))
        label[0].place(x = 750, y = 370)
        create_tool_tip(label[0], text='Number of edges to attach from a new\nnode to existing nodes')
        network_parameters.append(tk.Entry(window,width=5))
        network_parameters[0].place(x = 800, y = 370)
        network_parameters[0].insert(tk.END, '1')
        information.insert(tk.INSERT, '\nNETWORK : BARABASI, A.; ALBERT, R. Emergence of scaling in random networks. Science, v. 286, n. 5439, p. 509-512, 1999.\n')

    elif network == "Modular":
        label.append(tk.Label(window, text="p: "))
        label[0].place(x = 750, y = 370)
        create_tool_tip(label[0], text='Probability of connection')
        label.append(tk.Label(window, text="r: "))
        label[1].place(x = 950, y = 370)
        create_tool_tip(label[1], text='Proportion of links within modules\ncompared to links across')
        label.append(tk.Label(window, text="M: "))
        label[2].place(x = 750, y = 400)
        create_tool_tip(label[2], text='Number of modules')
        network_parameters.append(tk.Entry(window,width=5))
        network_parameters[0].place(x = 800, y = 370)
        network_parameters[0].insert(tk.END, '1')
        network_parameters.append(tk.Entry(window,width=5))
        network_parameters[1].place(x = 1000, y = 370)
        network_parameters[1].insert(tk.END, '1')
        network_parameters.append(tk.Entry(window,width=5))
        network_parameters[2].place(x = 800, y = 400)
        network_parameters[2].insert(tk.END, '1')
        information.insert(tk.INSERT, '\nNETWORK : SAH, P. et al. Exploring community structure in biological networks with random graphs. BMC bioinformatics, v. 15, n. 1, p. 220, 2014.\n')

    elif network == "Hierarchical":
        label.append(tk.Label(window, text="b: "))
        label[0].place(x = 750, y = 370)
        create_tool_tip(label[0], text='Tree branch factor')
        label.append(tk.Label(window, text="m: "))
        label[1].place(x = 950, y = 370)
        create_tool_tip(label[1], text='Number of additional edges to add')
        label.append(tk.Label(window, text=u'\u03BB: '))
        label[2].place(x = 750, y = 400)
        create_tool_tip(label[2], text='Parameter in [0,inf)')
        label.append(tk.Label(window, text=u'\u03BE: '))
        label[3].place(x = 950, y = 400)
        create_tool_tip(label[3], text='Parameter in [0,inf)')
        network_parameters.append(tk.Entry(window,width=5))
        network_parameters[0].place(x = 800, y = 370)
        network_parameters[0].insert(tk.END, '2')
        network_parameters.append(tk.Entry(window,width=5))
        network_parameters[1].place(x = 1000, y = 370)
        network_parameters[1].insert(tk.END, '0')
        network_parameters.append(tk.Entry(window,width=5))
        network_parameters[2].place(x = 800, y = 400)
        network_parameters[2].insert(tk.END, '1')
        network_parameters.append(tk.Entry(window,width=5))
        network_parameters[3].place(x = 1000, y = 400)
        network_parameters[3].insert(tk.END, '1')
        information.insert(tk.INSERT, '\nNETWORK: DODDS, P. S.; WATTS, D. J.; SABEL, C. F. Information exchange and the robustness of organizational networks. Proceedings of the National Academy of Sciences, v. 100, n. 21, p. 12516-12521, 2003.\n')

    else :
        open_file(window)
    information['state'] = tk.DISABLED

    text_after = len(information.get("1.0", tk.END))

    aux_info=text_after-text_before
    btn1['state'] = tk.NORMAL

def open_file(window):
    global size_arq

    filename.clear()
    filename.append(filedialog.askopenfilename(initialdir=os.getcwd(),title = "Select File", filetypes=(("dat files",".dat"), ("txt files",".txt"), ("all files","*.*"))))
    arq = np.loadtxt(filename[0])

    max_arq = np.amax(arq,axis=0)
    min_arq = np.amin(arq,axis=0)

    if max_arq[1]>max_arq[0]:
        if min_arq[0]<min_arq[1]:
            size_arq = max_arq[1] - min_arq[0] + 1
        else :
            size_arq = max_arq[1] - min_arq[1] + 1
    else:
        if min_arq[0]<min_arq[1]:
            size_arq = max_arq[0] - min_arq[0] + 1
        else :
            size_arq = max_arq[0] - min_arq[1] + 1

def plot_results(answer,model):
    aux=[]
    
    MA = loadtxt('IBM_result.dat')
    plt.figure('IBM results (fig' + str(len(plt.get_fignums()) + 1) + ')')

    if answer=="no":
        a = ''
        for i in range(0,len(entries_compartments)):
            plt.plot(MA[:, 0], MA[:, i+1],color = colors[i])
            aux.append(str(entries_compartments[i].get()))
            if i == len(entries_compartments)-1:
                a = a + str(entries_compartments[i].get())+ '(t)'
            else:
                a = a + str(entries_compartments[i].get())+ '(t), '
        plt.legend(aux)
        plt.ylabel(a)
    else:
        a = ''
        for i in range(0,len(compartment_models[model.current()])):
            plt.plot(MA[:, 0], MA[:, i+1],color = colors[i])
            aux.append(str(compartment_models[model.current()][i]))
            if i == len(compartment_models[model.current()])-1:
                a = a + str(compartment_models[model.current()][i])+ '(t)'
            else:
                a = a + str(compartment_models[model.current()][i])+ '(t), '
        plt.legend(compartment_models[model.current()])
        plt.ylabel(a)
    plt.title('Number of individuals in time')
    plt.xlabel('t')
    plt.grid()
    plt.show()

def show_graph(graph_chosen,answer,model):
    file  = open("network.dat","r")
    graph = nx.read_edgelist(file)
    file.close()

    ans = 0
    for i in range(0,len(initial_condition)):
        ans = ans + int(initial_condition[i].get())

    for i in range(0,ans):
        aux=0
        for j in graph:
            if int(j) == i:
                aux=1
                break
        if aux==0:
            graph.add_node(str(i))


    if graph_chosen.get() == "spring":
        pos=nx.spring_layout(graph)
    elif graph_chosen.get() == "circular":
        pos=nx.circular_layout(graph)
    elif graph_chosen.get() == "random":
        pos=nx.random_layout(graph)
    elif graph_chosen.get() == "shell":
        pos=nx.shell_layout(graph)
    elif graph_chosen.get() == "spectral":
        pos=nx.spectral_layout(graph)
    elif graph_chosen.get() == "fruchterman reingold":
        pos=nx.fruchterman_reingold_layout(graph)

    file2 = open("color_graph.dat","r")
    v  = []
    v0 = []
    v1 = []
    v2 = []
    v3 = []
    v4 = []
    v5 = []
    v6 = []
    v7 = []
    v8 = []
    v9 = []

    for lines in file2.readlines():
        cor = lines.strip().split()
        if cor[1] == '0':
            v0.append(str(cor[0]))
        if cor[1] == '1':
            v1.append(str(cor[0]))
        if cor[1] == '2':
            v2.append(str(cor[0]))
        if cor[1] == '3':
            v3.append(str(cor[0]))
        if cor[1] == '4':
            v4.append(str(cor[0]))
        if cor[1] == '5':
            v5.append(str(cor[0]))
        if cor[1] == '6':
            v6.append(str(cor[0]))
        if cor[1] == '7':
            v7.append(str(cor[0]))
        if cor[1] == '8':
            v8.append(str(cor[0]))
        if cor[1] == '9':
            v9.append(str(cor[0]))

    v.append(v0)
    v.append(v1)
    v.append(v2)
    v.append(v3)
    v.append(v4)
    v.append(v5)
    v.append(v6)
    v.append(v7)
    v.append(v8)
    v.append(v9)

    if answer.get()=="no":
        for i in range(0,len(entries_compartments)):
            nx.draw_networkx_nodes(graph, pos, nodelist=[ele for ele in v[i]], node_size=5, node_color = colors[i],label=str(entries_compartments[i].get()))
    else:
        for i in range(0,len(compartment_models[model.current()])):
            nx.draw_networkx_nodes(graph, pos, nodelist=[ele for ele in v[i]], node_size=5, node_color = colors[i],label=str(compartment_models[model.current()][i]))
 
    nx.draw_networkx_edges(graph,pos, width=0.25, edge_color='grey')
    plt.legend()
    plt.axis('off')
    plt.show()

def show_measures():
    window_measures = tk.Tk()
    window_measures.geometry('520x220')
    window_measures.title("Measurements")

    label2 = []
    label3 = []

    output = popen('measures.exe network.dat').read()
    file = open("measures.dat","r")
    measures = []

    for lines in file.readlines():
        measures.append(lines.strip().split())
    label2.append(tk.Label(window_measures, text=u'\u2329' + " k " + u'\u232a' + " : " ))
    label2[0].place (x=10,y=10)
    label3.append(tk.Label(window_measures, text=measures[0][0]))
    label3[0].place (x=100,y=10)
    create_tool_tip(label2[0], text="Average Degree")

    label2.append(tk.Label(window_measures, text="d : " ))
    label2[1].place (x=10,y=40)
    label3.append(tk.Label(window_measures, text=measures[0][1]))
    label3[1].place (x=100,y=40)
    create_tool_tip(label2[1], text="Density")

    label2.append(tk.Label(window_measures, text="CC : "))
    label2[2].place (x=10,y=70)
    label3.append(tk.Label(window_measures, text=measures[0][2]))
    label3[2].place (x=100,y=70)
    create_tool_tip(label2[2], text="Average Clustering coefficient")

    label2.append(tk.Label(window_measures, text=u'\u2329' + " L " + u'\u232a' + " : "  ))
    label2[3].place (x=290,y=10)
    label3.append(tk.Label(window_measures, text=measures[0][3]))
    label3[3].place (x=380,y=10)
    create_tool_tip(label2[3], text="Average Shortest path length")

    label2.append(tk.Label(window_measures, text="efficiency : " ))
    label2[4].place (x=290,y=40)
    label3.append(tk.Label(window_measures, text=measures[0][4]))
    label3[4].place (x=380,y=40)
    create_tool_tip(label2[4], text="Efficiency of network")

    label2.append(tk.Label(window_measures, text="diameter : " ))
    label2[5].place (x=290,y=70)
    label3.append(tk.Label(window_measures, text=measures[0][5]))
    label3[5].place (x=380,y=70)
    create_tool_tip(label2[5], text="Diameter of network")

    tk.Label(window_measures, text="Information" ).place (x=10,y=110)
    info = scrolledtext.ScrolledText(window_measures, wrap = tk.WORD, width=60, height=4)
    info.insert(tk.INSERT, 'COSTA, L. F. et al. Characterization of complex networks: A survey of measurements. Advances in physics, v. 56, n. 1, p. 167-242, 2007.')
    info.place (x=10,y=130)
    info['state'] = tk.DISABLED

def teste(btn2,btn3,dinamics_vital,network,answer):
    if answer.get()=="yes":
        run_simulation(btn2,btn3,dinamics_vital,network,answer.get(),label2[1])
    else:
        run_simulation(btn2,btn3,dinamics_vital,network,answer.get(),1000)
    
def create_window():
    window = tk.Tk()
    window.geometry('1500x700')
    window.title("IBM GUI")

    tk.Label(window,text="Information ").place(x = 10, y = 590)
    information = scrolledtext.ScrolledText(window, wrap = tk.WORD, height=4,width = 165)
    information.place (x=10,y=610)
    information['state'] = tk.DISABLED

    tk.Label(window,text="Defined models ").place(x = 10, y = 10)
    model_defined = ttk.Combobox(window, values=["yes","no"], width=5,state="readonly")
    model_defined.set("")
    model_defined.place(x = 200, y = 10)

    model_defined.bind("<<ComboboxSelected>>", lambda event: defined_models(window,model_defined,information,btn1,btn2,btn3))

    tk.Label(window, text="Values Parameters: ").place(x = 750, y = 10)

    tk.Label(window, text="Vital Dynamic: ").place(x = 750, y = 130)
    dinamics_vital = ttk.Combobox(window, values=["without","within"],width=7,state="readonly")
    dinamics_vital.current(0)
    dinamics_vital.place(x = 950, y = 130)
    dinamics_vital.bind("<<ComboboxSelected>>", lambda event: dinamics_vital_function(window,dinamics_vital))

    configuration_simulation(window)

    tk.Label(window, text="Network: ").place(x = 750, y = 340)
    network = ttk.Combobox(window, values=["Random","Small-World","Scale-Free","Modular","Hierarchical", "other"],width=20,state="readonly")
    network.set("")
    network.place(x = 950, y = 340)
    network.bind("<<ComboboxSelected>>", lambda event: network_chosen(window,network,btn1,information))

    btn1 = tk.Button(window, text='Run simulation', bg='light green', state=tk.DISABLED, command = lambda: teste(btn2,btn3,dinamics_vital,network,model_defined))
    btn1.place(x = 600, y = 550)

    tk.Label(window, text="Layout Graph: ").place(x = 750, y = 460)
    layout_graph = ttk.Combobox(window, values=["fruchterman reingold", "spectral", "shell", "random", "circular", "spring"],width=20,state="readonly")
    layout_graph.current(0)
    layout_graph.place(x = 950, y = 460)
    btn2 = tk.Button(window, text='Show Graph', state=tk.DISABLED,command=lambda: show_graph(layout_graph,model_defined,label2[1]))
    btn2.place(x = 800, y = 490)

    btn3 = tk.Button(window, text='Show Measures', state=tk.DISABLED,command=lambda: show_measures())
    btn3.place(x = 950, y = 490)
    
    return window

def chosen_model(window,model,information,btn1,btn2,btn3):
    btn2['state'] = tk.DISABLED
    btn3['state'] = tk.DISABLED
    for i in initial_condition:
        i.delete(0, tk.END)
        i.place_forget()
    
    for i in label21:
        i.place_forget()
    label21.clear()

    for i in name_compartment2:
        i.place_forget()

    for i in config_values:
        i.delete(0, tk.END)

    for i in value_dv:
        i.delete(0, tk.END)

    for i in entries_values_parameters:
        i.delete(0, tk.END)
        i.place_forget()
    
    for i in name_parameter:
        i.place_forget()

    entries_values_parameters.clear()
    name_parameter.clear()
    initial_condition.clear()
    name_compartment2.clear()

    model_img = Image.open("models/" + str(model.current()) + '.png')
    model_img = ImageTk.PhotoImage(model_img)
    label21.append(tk.Label(window,image=model_img))
    label21[0].image = model_img
    label21[0].place(x = 10, y = 150)

    label21.append(tk.Label(window, text="Initial Condition: "))
    label21[1].place(x = 460, y = 220)
    ode_system(window,model.current())

    global aux_info

    information['state'] = tk.NORMAL
    information.delete(1.0, tk.END)
    aux_info = 0
    if model.current()==0:
        information.insert(tk.INSERT, 'MODEL : HETHCOTE, H. W. Three basic epidemiological models. In: Applied mathematical ecology. Springer, Berlin, Heidelberg, 1989. p. 119-144.')
    elif model.current()==1:
        information.insert(tk.INSERT, 'MODEL : HETHCOTE, H. W. Three basic epidemiological models. In: Applied mathematical ecology. Springer, Berlin, Heidelberg, 1989. p. 119-144.')
    elif model.current()==2:
        information.insert(tk.INSERT, 'MODEL : KERMACK, W. O.; MCKENDRICK, A. G. A contribution to the mathematical theory of epidemics. Proceedings of the royal society of london. Series A, Containing papers of a mathematical and physical character, v. 115, n. 772, p. 700-721, 1927.')
    elif model.current()==3:
        information.insert(tk.INSERT, 'MODEL : HETHCOTE, H. W. Qualitative analyses of communicable disease models. Mathematical Biosciences, v. 28, n. 3-4, p. 335-356, 1976.')
    elif model.current()==4:
        information.insert(tk.INSERT, 'MODEL : LONGINI, I. M.; BAILEY, N. T. J. The generalized discrete-time epidemic model with immunity. In: Vol. 2 Mathematical Statistics Theory and Applications. De Gruyter, 2020. p. 547-552.')
    else:
        information.insert(tk.INSERT, 'MODEL : LONGINI, I. M.; BAILEY, N. T. J. The generalized discrete-time epidemic model with immunity. In: Vol. 2 Mathematical Statistics Theory and Applications. De Gruyter, 2020. p. 547-552.')
    information['state'] = tk.DISABLED

    value_parameters(window,model.current())

    config_values[0].insert(tk.END, '10')
    config_values[1].insert(tk.END, '0.1')
    config_values[2].insert(tk.END, '100')

def defined_models(window,answer,information,btn1,btn2,btn3):
    btn2['state'] = tk.DISABLED
    btn3['state'] = tk.DISABLED
    for i in label2:
        i.place_forget()
    label2.clear()

    for i in label21:
        i.place_forget()
    label21.clear()

    for i in entries_compartments:
        i.delete(0, tk.END)
        i.place_forget()
    
    for i in initial_condition:
        i.delete(0, tk.END)
        i.place_forget()

    for i in entries_ode:
        i.delete(0, tk.END)
        i.place_forget()

    for i in name_compartment:
        i.place_forget()

    for i in name_compartment2:
        i.place_forget()

    for i in entries_parameters:
        i.delete(0, tk.END)
        i.place_forget()

    for i in entries_values_parameters:
        i.delete(0, tk.END)
        i.place_forget()
    
    for i in name_parameter:
        i.place_forget()

    entries_parameters.clear()
    entries_values_parameters.clear()
    name_parameter.clear()
    entries_compartments.clear()
    initial_condition.clear()
    entries_ode.clear()
    name_compartment.clear()
    name_compartment2.clear()

    if answer.get() == "yes":
        label2.append(tk.Label(window,text="Choose the Model"))
        label2[0].place(x = 10, y = 40)
        label2.append(ttk.Combobox(window, values=['SI', 'SIS', 'SIR', 'SIRS', 'SEIR', 'SEIRS'], width=5,state="readonly"))
        label2[1].set("")
        label2[1].place(x = 200, y = 40)
        label2[1].bind("<<ComboboxSelected>>", lambda event: chosen_model(window,label2[1],information,btn1,btn2,btn3))
        label2.append(tk.Label(window, text="ODE system:"))
        label2[2].place(x = 10, y = 70)

    else :
        information['state'] = tk.NORMAL
        information.delete(1.0, tk.END)
        aux_info = 0
        information['state'] = tk.DISABLED

        label2.append(tk.Label(window,text="Number of compartments: "))
        label2[0].place(x = 10, y = 40)
        label2.append(ttk.Combobox(window, values=[2, 3, 4, 5, 6, 7, 8, 9, 10], width=5,state="readonly"))
        label2[1].set("")
        label2[1].place(x = 200, y = 40)
        label2.append(tk.Label(window, text="Compartments names: "))
        label2[2].place(x = 10, y = 70)
        label2[1].bind("<<ComboboxSelected>>", lambda event: compartments_names(window,label2[1]))

        label2.append(tk.Label(window, text="Number of parameters: "))
        label2[3].place(x = 10, y = 130)
        label2.append(ttk.Combobox(window, values=[1, 2, 3, 4, 5, 6, 7, 8, 9, 10],width=5,state="readonly"))
        label2[4].set("")
        label2[4].place(x = 200, y = 130)
        label2.append(tk.Label(window, text="Parameters names: "))
        label2[5].place(x = 10, y = 160)
        label2[4].bind("<<ComboboxSelected>>", lambda event: parameters_names(window,label2[4]))

        label2.append(tk.Button(window, text="Writer system", command = lambda:writer_system(window)))
        label2[6].place(x = 10, y = 190)

        label2.append(tk.Label(window, text="ODE system: "))
        label2[7].place(x = 10, y = 220)
        label2.append(tk.Label(window, text="Initial Condition: "))
        label2[8].place(x = 460, y = 220)
    
if __name__ == "__main__":
    create_window().mainloop()
    for file in ['INPUT_IBM.dat','IBM.dat','network.dat','color_graph.dat','measures.dat','IBM_result.dat']:
        if path.isfile(file):
            remove(file)

