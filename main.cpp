#include <iostream>
#include <string>
#include <string.h>
#include <vector>
using namespace std;

class Geometry
{
   int m_x;
   int m_y;
public:
   Geometry(int x, int y) {
        m_x= x;
        m_y= y;
   }

   virtual string getString()=0;
   int getX(){return m_x;}
   int getY(){return m_y;}
};

class Circle2D : public Geometry
{
    int m_r;
public:
    Circle2D(int x, int y, int r) : Geometry( x, y)
    {
        m_r=r;
    }
    string getString(){
        return string("Circle2D x:") + to_string(getX()) +string(" y:")+to_string(getY())+string(" Radius:") + to_string(m_r);
    }

};

class Rectangle2D : public Geometry
{
   int m_width;
   int m_heigh;
   
public:
    Rectangle2D(int x, int y, int w, int h): Geometry( x, y)
    {
        m_width = w;
        m_heigh = h; 
    }
    string getString(){
        return string("Rectangle2D x:") +to_string(getX())+string(" y:")+to_string(getY())+string(" Width:") + to_string(m_width) +string(" Height:") + to_string(m_heigh);}
   
};

class Circle3D : public Geometry
{
    int m_r;
public:
   Circle3D(int x, int y, int r):Geometry( x,  y)
    {
       m_r = r;
    }
    string getString(){
    return string("Circle3D x:") + to_string(getX()) +string(" y:")+to_string(getY())+string(" Radius:") + to_string(m_r);}
    
};

class Rectangle3D : public Geometry
{
    int m_width;
    int m_heigh;
public:
    Rectangle3D(int x, int y, int w, int h): Geometry( x,  y)
    {
     m_width = w;
     m_heigh = h;
    }
    string getString(){
    return string("Rectangle3D x:") +to_string(getX())+string(" y:")+to_string(getY())+string(" Width:") + to_string(m_width) +string(" Height:") + to_string(m_heigh);}
    
};

class GeometryFactory
{
public:
 virtual Geometry* CreateCircle(int x, int y, int r)=0;
 virtual Geometry* CreateRectangle(int x, int y, int w, int h)=0;
};

class Geometry2D : public GeometryFactory
{

public:

     Geometry* CreateCircle(int x, int y, int r)
     {
         return new Circle2D(x,y,r);
     }
      Geometry* CreateRectangle(int x, int y, int w, int h)
     {
         return new Rectangle2D(x,y,w,h);
     }

};

class Geometry3D : public GeometryFactory
{

public:

    Geometry* CreateCircle(int x, int y, int r)
    {
        return new Circle3D(x,y,r);
    }
    Geometry* CreateRectangle(int x, int y, int w, int h)
    {
        return new Rectangle3D(x,y,w,h);
    }
};

class User
{
    GeometryFactory* m_f;
    vector<Geometry*> m_g2D;
    vector<Geometry*> m_g3D;
    vector<vector<Geometry*>> m_h2D;
    vector<vector<Geometry*>> m_h3D;
    int m_lenght;
    int temp;
public:
    User()
    {
        m_lenght =0;
        temp = 0;
    }
    vector<Geometry*> getGeometry2D()
    {
        return m_g2D;
    }
    vector<Geometry*> getGeometry3D()
    {
        return m_g3D;
    }
    void AddCircle(int x, int y, int r)
    {
        m_f = new Geometry2D();
        m_g2D.push_back(m_f->CreateCircle(x,y,r));
        m_f = new Geometry3D();
        m_g3D.push_back(m_f->CreateCircle(x,y,r));

        m_h2D.push_back(m_g2D);
        m_h3D.push_back(m_g3D);

        m_lenght +=1;
        temp = m_lenght;
    }
    void AddRectangle(int x, int y, int w, int h)
    {
        m_f = new Geometry2D();
        m_g2D.push_back(m_f->CreateRectangle(x,y,w,h));
        m_f = new Geometry3D();
        m_g3D.push_back(m_f->CreateRectangle(x,y,w,h));

        m_h2D.push_back(m_g2D);
        m_h3D.push_back(m_g3D);

        m_lenght +=1;
        temp = m_lenght;
    }
    bool Undo()
    {

        temp -=1;
        if(temp >=1)
        {
            m_g2D = m_h2D[temp-1];
            m_g3D = m_h3D[temp-1];
            return true;
        }
        else if(temp==0)
        {
           m_g2D.clear();
           m_g3D.clear();
           return true;
        }

        else
        {
             temp = 0;
             return false;
        }
    }
    bool Redo()
    {

          temp +=1;
        if(temp <=m_lenght)
        {
            m_g2D = m_h2D[temp-1];
            m_g3D = m_h3D[temp-1];
            return true;
        }
        else // temp > m_lenght do bam nhieu lan undo
        {
            temp = m_lenght;
            return false;
        }
    }
    bool Remove(int x, int y)
    {
         if (temp == m_lenght)
         {

             for (int i=0; i<= m_g2D.size()-1; i++)
            {

                if (m_g2D[i]->getX()==x && m_g2D[i]->getY()==y)
                {
                      m_g2D.erase(m_g2D.begin()+i);// xoa dung roi
                      m_g3D.erase(m_g3D.begin()+i);
                      cout <<"huy\n";
                      if(m_g2D.size())
                      {
                          m_h2D.push_back(m_g2D);
                          m_h3D.push_back(m_g3D);
                          m_lenght +=1;
                          temp = m_lenght;
                          return true;
                      }
                      else
                      {
                          m_h2D[temp-1]= m_g2D;
                          m_h3D[temp-1]= m_g3D;
                          return true;
                      }
                }
            }
         }
         else if (temp < m_lenght)
         {
             m_h2D.erase(m_h2D.begin()+ temp, m_h2D.end());
             m_h3D.erase(m_h3D.begin()+ temp, m_h3D.end());
             m_lenght = m_h2D.size();
             m_g2D = m_h2D[temp-1];
             m_g3D = m_h3D[temp-1];
              for (int i=0; i<= m_g2D.size()-1; i++)
            {

                if (m_g2D[i]->getX()==x && m_g2D[i]->getY()==y)
                {
                      m_g2D.erase(m_g2D.begin()+i);
                      m_g3D.erase(m_g3D.begin()+i);
                      m_h2D.push_back(m_g2D);
                      m_h3D.push_back(m_g3D);
                      m_lenght +=1;
                      temp = m_lenght;
                    return true;
                }
            }
         }

        return false;
    }
};

void Menu()
{
    cout<<"1.Add\n";
    cout<<"2.Remove\n";
    cout<<"3.Undo\n";
    cout<<"4.Redo\n";
    cout<<"5.Print\n";
    cout<<"6.Exit\n";
    cout<<">";

}
void Print(vector<Geometry*> vecg)
{
   if(vecg.size()!=0)
   {
       for (int i=0; i<= vecg.size()-1; i++)
        {
       cout<< vecg[i]->getString() <<'\n';
        }
   }
   else
        cout <<"Emty\n";

}


int main()
{
    int input;
     int x,y,r,w,h;
     User user1;
     while(1)
     {

         Menu();
         cin >> input;
         switch(input)
         {
         case 1:
             cout<<"1.Circle\n";
             cout<<"2.Rectangle\n";
             cout<<">";
             cin >> input;
             if(input==1)
            {
                cout<<"Input x: ";
                cin >> x;
                cout<<"Input y: ";
                cin >> y;
                cout<<"Input Radius: ";
                cin >> r;
                user1.AddCircle(x,y,r);
            }
            else if (input==2)
            {
                cout<<"Input x: ";
                cin >> x;
                cout<<"Input y: ";
                cin >> y;
                cout<<"Input Width: ";
                cin >> w;
                cout<<"Input Height: ";
                cin >> h;
                user1.AddRectangle(x,y,w,h);
            }
            system("cls");

            break;
         case 2:
             cout <<"Input x:";
             cin >> x;
             cout <<"Input y:";
             cin >> y;
             system("cls");
             if(user1.Remove(x, y)==true)
                cout <<"Remove successed\n";
             else cout << "Remove failed\n";

            break;
         case 3:
             if(user1.Undo()==true)
             {
                 cout <<"Undo Successed\n";
             }
             else
                 cout <<"Undo Failed\n";
            break;
         case 4:
             if(user1.Redo()==true)
             {
                 cout <<"Redo Successed\n";
             }
             else
                 cout <<"Redo Failed\n";
            break;
         case 5:
             cout <<"1.Print 2D\n";
             cout <<"2.Print 3D\n";
             cout <<">";
             cin >> input;
             system("cls");
             if (input==1)

             Print(user1.getGeometry2D());

             else if (input==2)

             Print(user1.getGeometry3D());

             break;
         case 6:
             exit(0);
             break;
         }

     }//while

    return 0;
}

