/**
* class name: OchiaghaifeanyiA5Q1
*
* COMP 2140 SECTION D01
* ASSIGNMENT Assignment 5, question 1
* @author Ochiagha ifeanyi , 7900465
* @version 13 August 2023
*
* PURPOSE: it is an emergency room simulation using a priority queue 
*/


import java.io.File;
import java.util.Scanner;

public class OchiaghaifeanyiA5Q1 {
    public static void main(String[] args){
        System.out.println("enter file name");
        Scanner fileName = new Scanner(System.in);
        String name = fileName.nextLine();
        Simulation(name);
    }

    public static void Simulation(String name){
        File file = new File(name);
        PriorityQueue one = new PriorityQueue();
        try {
             Scanner scan = new Scanner(file);
            int count = 1;
            int time = 0;
            int doctorFree = 0;
            String line = scan.nextLine();
            String[] wordsArray;
            int treatTime;
            int arrivalTime = 0;
            boolean isFirstIteration = true;
            while(scan.hasNextLine() || !one.isEmpty()){//this makes sure the loop keeps running until both conditions are fulfiled
                
                if(doctorFree == time ){
                    System.out.println("Doctor is available at time = " + doctorFree);
                    if(!one.isEmpty()){
                        Patient temp = one.deleteMax();
                         System.out.println("Patient " + temp.patientNum + " in for treatment at time = " + doctorFree + " with urgency = " + temp.urgency + " and treatment time = " + temp.treatment );
                        doctorFree += temp.treatment;
                       
                    }
                    
                }
               
                wordsArray = line.split("\\s");
                treatTime = Integer.parseInt(wordsArray[2]);
                int urgency = Integer.parseInt(wordsArray[1]);
                arrivalTime = Integer.parseInt(wordsArray[0]);

            //    if(arrivalTime > doctorFree && time < doctorFree && !isFirstIteration){
            //         time = doctorFree;
            //     }
            //     else{
                    time =  arrivalTime;
            //    }

                if(isFirstIteration){//this checks if it is reading the first line and carries out the below code
                    doctorFree += time;
                    isFirstIteration = false;
                }
                // else if(time < doctorFree){
                //     int difference = doctorFree - time;
                //     time += difference;
                // }

                if(time == arrivalTime ){
                    Patient newPatient = new Patient(count, urgency,treatTime );
                    one.insert(newPatient, time);
                    
                    
                    if (!scan.hasNextLine()) {
                        while (!one.isEmpty()) {//this ensures all the patients are treated before exiting the loop
                            if (doctorFree == time) {
                                System.out.println("Doctor is available at time = " + doctorFree);
                                Patient temp = one.deleteMax();
                                System.out.println("Patient " + temp.patientNum + " in for treatment at time = " + doctorFree + " with urgency = " + temp.urgency + " and treatment time = " + temp.treatment);
                                doctorFree += temp.treatment;
                            }
                            time = doctorFree;
                        }
                        break;
                    }


                    if (scan.hasNextLine()) {
                            line = scan.nextLine();
                            String[] check = line.split("\\s");
                            if(Integer.parseInt(check[0]) > doctorFree){
                                time = doctorFree;//this skips to the event when the doctor is free to treat another patient
                            }
                            count++;
                    }
                    else if(!one.isEmpty()){
                        time = doctorFree;
                     }
                     
                }
              
              
            }
             
             scan.close();
        }
        catch (Exception e) {
            // TODO: handle exception
            e.getMessage();
        }
        

    }
}

class PriorityQueue {
     Patient[] heapArray = new Patient[1000];
    private int count = 0;
    
    public void insert(Patient newPatient, int time){
        if(count < heapArray.length ){
            System.out.println(newPatient.toString(newPatient, time));
            heapArray[count] = newPatient;
            count++;
            siftUp( count - 1);
        }
      
        
    }

    public boolean isEmpty(){
        if(count == 0){
            return true;
        }
        return false;
    }

    
    public Patient Peek(){
        return heapArray[0];
    }

    public Patient getMax(){
        if(count == 0){
            return null;
        }
        return heapArray[0];
    }
    public Patient deleteMax(){
        if(count == 0){
            return null;
        }
        Patient toReturn = heapArray[0];
        swap(0 , count - 1);
        count--;
        siftDownHelp(0);
        return toReturn;
    }


    private void siftUp(int index){
        int i = index;
        while(i > 0 && heapArray[(i - 1)/2].urgency < heapArray[i].urgency){
            swap(i, (i - 1)/2);
            i = (i - 1)/2;

        }
    }

    private boolean inside(int num){
        for(int i = 0; i < count; i++){
            if(heapArray[i].patientNum == num){
                return true;
            }
        }

        return false;

    }


    private void siftDownHelp(int index) {
    int child = 2 * index + 1;
    int value = heapArray[index].urgency;

    while (child < count) {
        int maxValue = value;
        int maxIndex = -1;
        for (int i = 0; i < 2 && i + child < count; i++) {
            if (heapArray[i + child].urgency > maxValue) {
                maxValue = heapArray[i + child].urgency;
                maxIndex = i + child;
            }
        }
        if (maxValue == value) {
            return;
        } else {
            swap(index, maxIndex);
            index = maxIndex;
            child = 2 * index + 1;
        }
    }
    }

    private int leftChild(int num){
        return (2 * num) + 1;
    }

    private int rightChild(int num){
        return (2 * num) + 2;

    }
    private void swap(int pos1, int pos2){
        Patient temp = heapArray[pos1];
        heapArray[pos1] = heapArray[pos2];
        heapArray[pos2] = temp;
    }

    private boolean isMaxheap(){
        boolean isMaxheap = false;
       // while(temp < count){
        for(int i = 0; i < count ; i++){
           // System.out.println(leftChild(i) + "mess");
            if(leftChild(i) < count && rightChild(i) < count ){
               // System.out.println(i);
                if(heapArray[i].urgency > heapArray[leftChild(i)].urgency && heapArray[i].urgency > heapArray[rightChild(i)].urgency){
                        isMaxheap = true;
                }
                else{
                    return false;
                }
        }
    }
  
    return  isMaxheap;
    }

    private void print(){
        for(int i = 0; i < count; i++){
            System.out.println(heapArray[i].urgency);
         }
  }

 
}

class Patient{
    int patientNum = 1;
    int urgency;
    int treatment;

    public Patient(int num, int urgency, int treat){
        patientNum = num;
        this.urgency = urgency;
        treatment = treat;
    }

    public int getNum(Patient temp){
        return temp.patientNum;
    }

    public int getUrgency(Patient temp){
        return temp.urgency;
    }

     public int getTreatNum(Patient temp){
        return temp.treatment;
    }

    public String toString(Patient temp, int time){
        String num = "Patient " + temp.patientNum + " arrived at time = " + time + " with urgency = " + temp.urgency+  " and treatment time = " +  temp.treatment;
        return num;
    }
}
